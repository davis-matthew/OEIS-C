/*
 * A133234  a(n) is least semiprime (not already in list) such that no 3-term
 *          subset forms an arithmetic progression.
 * https://oeis.org/A133234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133234: a(n) = least semiprime not forming an AP with prior terms.
   The function performs a brute‑force search, keeping the required terms in
   a static array and expanding a small sieve of smallest prime factors as
   needed.  The algorithm is O(n^2) in the number of produced terms, which
   is fast enough for n up to about 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Global storage for the sequence and for a sieve of smallest prime factors */
static seq_t  *seq       = NULL;   /* accumulated terms */
static size_t seq_cap   = 0;       /* allocation size for seq */
static size_t seq_len   = 0;       /* number of terms stored */

/* Smallest prime factor array: spf[i] == 0 means i is prime (or i<2) */
static uint32_t *spf      = NULL;
static idx_t    spf_size  = 0;

/* --------------------------------------------------------------------- */
/* Grow the sequence array when more space is required */
static void seq_grow(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_seq) exit(EXIT_FAILURE);
        if (seq) {
            for (size_t i = 0; i < seq_len; ++i)
                new_seq[i] = seq[i];
            free(seq);
        }
        seq = new_seq;
        seq_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Does the array contain x?  seq is sorted increasingly. */
static bool seq_contains(seq_t x, size_t len)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq[mid] < x)
            lo = mid + 1;
        else if (seq[mid] > x)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Ensure that spf covers all numbers up to 'limit' (inclusive).  A linear
   sieve is rebuilt from scratch when extending.  This is inexpensive
   because we only grow the sieve logarithmically. */
static void ensure_spf_limit(idx_t limit)
{
    if (limit < spf_size)
        return;

    idx_t new_size = (spf_size) ? spf_size << 1 : 1024;
    if (new_size <= limit)
        new_size = limit + 1;
    if (new_size < limit + 1)
        new_size = limit + 1;

    uint32_t *new_spf = (uint32_t *)calloc((size_t)new_size, sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    /* collect primes for the sieve */
    size_t prime_cap = new_size / 10 + 10;
    uint32_t *primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) { free(new_spf); exit(EXIT_FAILURE); }
    size_t primes_len = 0;

    for (uint32_t i = 2; i < (uint32_t)new_size; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (primes_len == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp) { free(new_spf); free(primes); exit(EXIT_FAILURE); }
                primes = tmp;
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > new_spf[i] || prod >= (uint64_t)new_size)
                break;
            new_spf[prod] = p;
        }
    }
    free(primes);
    free(spf);
    spf = new_spf;
    spf_size = new_size;
}

/* --------------------------------------------------------------------- */
/* Test whether n (≥2) is a product of exactly two primes.  Uses the
   smallest‑prime‑factor sieve.  Returns true if n = p·q with p and q prime. */
static bool is_semiprime(idx_t n)
{
    if (n <= 3) return false;          /* 4 is the first semiprime */
    ensure_spf_limit(n);
    uint32_t p = spf[n];
    if (p == 0 || p == n) return false;  /* n is prime or 1 */
    idx_t q = n / p;
    return (q >= 2 && spf[q] == q);      /* q must be prime */
}

/* --------------------------------------------------------------------- */
/* Compute the sequence up to at least index 'target' (1‑based).  The
   static variable 'candidate' remembers where we left off. */
static void compute_up_to(idx_t target)
{
    static idx_t candidate = 4;    /* first semiprime to test */

    if (seq_len == 0)          /* first time: allocate array space */
        seq_grow();

    while (seq_len < (size_t)target) {
        if (candidate >= spf_size)
            ensure_spf_limit(candidate + 1);

        if (!is_semiprime(candidate)) {
            ++candidate;
            continue;
        }

        seq_t cand = candidate;
        bool ok = true;
        for (size_t j = 0; j < seq_len; ++j) {
            seq_t aj = seq[j];
            seq_t two_aj = aj + aj;
            if (cand > two_aj)      /* candidate too large to make 2*aj - cand positive */
                continue;
            seq_t x = two_aj - cand;    /* 2*aj - cand */
            if (seq_contains(x, seq_len)) { /* would create an arithmetic progression */
                ok = false;
                break;
            }
        }

        if (ok) {      /* accept this semiprime */
            seq_grow();
            seq[seq_len++] = cand;
        }
        ++candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Public entry point: return the n‑th term of the sequence
   (a(1) is the first term).  Calls are allowed in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    compute_up_to(n);
    return seq[n - 1];
}
