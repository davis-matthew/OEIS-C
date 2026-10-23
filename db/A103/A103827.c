/*
 * A103827  Arithmetic means of the divisors of unitary arithmetic numbers
 *          (i.e., of those for which the arithmetic mean of the unitary
 *          divisors is an integer, A103826).
 * https://oeis.org/A103827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic mean of the unitary divisors * of the n‑th
 *            unitary arithmetic number. * A number m is unitary arithmetic
 *            iff * Π(1 + p_i^{e_i}) is divisible by 2^{k}, * where m = Π
 *            p_i^{e_i} and k is the number * of distinct prime factors. * The
 *            mean is then Π(1 + p_i^{e_i}) / 2^{k}
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Smallest prime factor sieve -------------------------------- */

static uint32_t *spf = NULL;    /* spf[i] = smallest prime factor of i */
static size_t   spf_limit = 0;  /* current length of spf   */

/* grow the sieve to at least new_limit (inclusive) */
static void grow_spf(size_t new_limit)
{
    uint32_t *new_spf = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    if (new_spf == NULL) abort();

    for (size_t i = 0; i <= new_limit; ++i) new_spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= new_limit; ++i)
    {
        if (new_spf[i] == (uint32_t)i)
        {
            for (size_t j = i * i; j <= new_limit; j += i)
                if (new_spf[j] == (uint32_t)j)
                    new_spf[j] = (uint32_t)i;
        }
    }
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ---------- Unit‑ary mean --------------------------------------------- */

static inline seq_t unitary_mean(uint32_t m)
{
    if (m == 1) return (seq_t)1;          /* 1 has mean 1 */

    uint64_t prod = 1;                    /* product of (1 + p^e) */
    int       k    = 0;                   /* number of distinct primes */
    uint64_t  v2   = 0;                   /* total power of 2 in prod */

    uint32_t x = m;
    while (x > 1)
    {
        uint32_t p = spf[x];
        ++k;
        uint64_t pe = 1;
        while (x % p == 0)
        {
            x /= p;
            pe *= (uint64_t)p;
        }
        uint64_t term = pe + 1;           /* (1 + p^e) */
        prod *= term;

        /* count factors of 2 in term */
        v2 += __builtin_ctzll(term);
    }

    if (v2 < (uint64_t)k) return (seq_t)0; /* mean not integral */

    seq_t numerator   = (seq_t)prod;
    seq_t divisor     = ((seq_t)1) << k;   /* 2^k */
    return numerator / divisor;
}

/* ---------- Memoisation of sequence a --------------------------------- */

static seq_t *memo = NULL;                 /* 0‑based store of a(n)  */
static size_t memo_cap = 0;                /* array capacity          */
static size_t memo_len = 0;                /* number of stored terms  */
static uint32_t next_num = 1;              /* next integer to test    */

/* ensure memo array has room for at least new_cap elements */
static void ensure_memo_cap(size_t new_cap)
{
    if (memo_cap >= new_cap) return;
    size_t new_size = memo_cap ? memo_cap * 2 : 16;
    while (new_size < new_cap) new_size <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (new_mem == NULL) abort();
    memo   = new_mem;
    memo_cap = new_size;
}

/* append a mean value to memo  */
static void add_mean(seq_t val)
{
    ensure_memo_cap(memo_len + 1);
    memo[memo_len++] = val;
}

/* initialisation of sieve and memoisation  */
static void initialise(void)
{
    if (spf == NULL) {
        grow_spf(2000000);   /* initial sieve size */
        next_num = 1;
    }
}

/* --------------------------- public function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds */

    initialise();

    /* generate terms until we have n entries */
    while (memo_len < (size_t)n)
    {
        /* ensure sieve covers the next number */
        if (next_num > spf_limit) {
            size_t new_limit = spf_limit * 2;
            grow_spf(new_limit);
        }

        seq_t m = unitary_mean(next_num);
        if (m != (seq_t)0) add_mean(m);

        ++next_num;
    }

    return memo[n-1];                     /* 0‑based array  */
}
