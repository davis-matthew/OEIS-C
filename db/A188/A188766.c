/*
 * A188766  Numbers n such that the number of decompositions of 2n into sum of
 *          two primes (counting 1 as a prime) is 1 or a composite.
 * https://oeis.org/A188766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n such that number of decompositions of 2*n into two primes
   (counting 1 as a prime) is 1 or a composite number.  The function
   builds the sequence lazily by sieving primes up to 2*n and counting
   unordered decompositions. */

/* ---- standard headers ---------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---- type definitions --------------------------------------------------- */
/* seq_t and idx_t are provided by the build; do not redefine them. */

/* ---- static state -------------------------------------------------------- */
static seq_t *seq          = NULL;   /* sequence values */
static idx_t seq_size      = 0;      /* number of stored terms */
static size_t seq_cap      = 0;      /* capacity of seq array */
static idx_t next_val      = 1;      /* next integer to test */

static idx_t *primes       = NULL;   /* list of primes up to sieve_limit */
static size_t prime_cnt    = 0;      /* count of primes */
static idx_t sieve_limit   = 0;      /* highest number that has been sieved */
static bool *is_prime      = NULL;   /* is_prime[i] == true if i is prime (1 considered prime) */

/* ---- helper to test small primes (used for decomposition counts) -------- */
static bool is_prime_small(unsigned v)
{
    if (v < 2) return false;
    for (unsigned i = 2; i * i <= v; ++i)
        if (v % i == 0) return false;
    return true;
}

/* ---- ensure that `seq` has at least `need` capacity --------------------- */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_seq = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    if (seq)
    {
        for (size_t i = 0; i < seq_size; ++i) new_seq[i] = seq[i];
        free(seq);
    }
    seq = new_seq;
    seq_cap = new_cap;
}

/* ---- sieve of Eratosthenes up to `limit`, 1 treated as prime ------------ */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;
    bool *new_is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!new_is_prime) abort();
    new_is_prime[0] = false;
    new_is_prime[1] = true;          /* 1 is considered prime */
    for (idx_t i = 2; i <= limit; ++i) new_is_prime[i] = true;
    for (idx_t i = 2; i <= limit / i; ++i)
        if (new_is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_is_prime[j] = false;

    /* rebuild prime list */
    free(primes);
    prime_cnt = 0;
    size_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (new_is_prime[i]) ++cnt;
    primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!primes) abort();
    for (idx_t i = 1; i <= limit; ++i)
        if (new_is_prime[i]) primes[prime_cnt++] = i;

    /* free old array */
    free(is_prime);
    is_prime = new_is_prime;
    sieve_limit = limit;
}

/* ---- count unordered decompositions of 2*m into two primes ------------ */
static seq_t count_decompositions(idx_t m)
{
    idx_t x = m * 2;
    seq_t cnt = 0;
    idx_t half = x / 2;
    for (size_t i = 0; i < prime_cnt && primes[i] <= half; ++i)
    {
        idx_t p = primes[i];
        idx_t q = x - p;
        if (q <= sieve_limit && is_prime[q]) ++cnt;
    }
    return cnt;
}

/* ---- grow the sequence until it has at least `n` terms ----------------- */
static void build_up_to(idx_t n)
{
    while (seq_size < n)
    {
        ensure_sieve(next_val * 2);
        seq_t cnt = count_decompositions(next_val);
        if (cnt == 1 || !is_prime_small((unsigned)cnt))
        {
            ensure_seq_cap(seq_size + 1);
            seq[seq_size++] = (seq_t)next_val;
        }
        ++next_val;
    }
}

/* ---- OEIS entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for n==0, but tests never call it */
    build_up_to(n);
    return seq[n - 1];
}
