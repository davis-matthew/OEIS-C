/*
 * A046928  Smaller of twin prime pairs in consecutively larger seas of
 *          composite numbers.
 * https://oeis.org/A046928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smaller prime of twin prime pair whose “sea of
 *            composites” has the nth distinct size. The sea size for a twin
 *            pair (p,p+2) is the number of consecutive composites between the
 *            prime preceding p and p itself plus the number of consecutive
 *            composites between p+2 and the next prime after p+2....
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Forward declaration – the build system provides the types seq_t and idx_t. */
extern seq_t a(idx_t n);

/* --------- Internals --------------------------------------- */

static seq_t *global_seq = NULL;        /* terms of the sequence in order of sea size */
static size_t global_seq_len = 0;       /* number of terms stored */
static idx_t global_limit = 1000000;   /* current sieve limit – doubled when needed */

/* Ensure that the sequence contains at least 'target' terms. */
static void ensure_sequence(idx_t target)
{
    /* If we already have enough terms, nothing to do. */
    if (global_seq_len >= (size_t)target)
        return;

    while (global_seq_len < (size_t)target)
    {
        /* ----------  Sieve of Eratosthenes up to global_limit  ---------- */
        idx_t L = global_limit;
        size_t sz = (size_t)(L + 1);
        unsigned char *is_prime = (unsigned char *)malloc(sz);
        if (!is_prime)
            return; /* out‑of‑memory; caller will get whatever it has */
        memset(is_prime, 1, sz);
        if (L >= 0) is_prime[0] = 0;
        if (L >= 1) is_prime[1] = 0;

        for (idx_t i = 2; i * i <= L; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= L; j += i)
                    is_prime[j] = 0;

        /* ----------  Collect primes into an array  ---------- */
        size_t primes_cap = 100000;
        uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        size_t primes_sz = 0;
        for (idx_t i = 2; i <= L; ++i)
            if (is_prime[i]) {
                if (primes_sz == primes_cap) {
                    primes_cap *= 2;
                    primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                }
                primes[primes_sz++] = (uint64_t)i;
            }

        /* ----------  Determine maximum sea size  ---------- */
        idx_t max_sea = 0;
        for (size_t k = 1; k + 1 < primes_sz; ++k) {
            if (primes[k + 1] == primes[k] + 2) { /* a twin pair */
                idx_t p  = primes[k];
                idx_t pv = primes[k + 1];        /* = p + 2 */
                idx_t pr = primes[k - 1];
                idx_t nx = primes[k + 2];
                idx_t sea = (p - pr - 1) + (nx - pv - 1);
                if (sea > max_sea)
                    max_sea = sea;
            }
        }

        /* ----------  Allocate minimal prime per sea size  ---------- */
        seq_t *min_prime = (seq_t *)calloc((size_t)(max_sea + 1), sizeof(seq_t));
        for (size_t k = 1; k + 1 < primes_sz; ++k) {
            if (primes[k + 1] == primes[k] + 2) {
                idx_t p  = primes[k];
                idx_t pv = primes[k + 1];
                idx_t pr = primes[k - 1];
                idx_t nx = primes[k + 2];
                idx_t sea = (p - pr - 1) + (nx - pv - 1);
                if (min_prime[sea] == 0)
                    min_prime[sea] = (seq_t)p;   /* store first (smallest) p for this sea */
            }
        }

        /* ----------  Build the sequence array  ---------- */
        size_t seq_new_len = 0;
        for (idx_t s = 1; s <= max_sea; ++s)
            if (min_prime[s] != 0)
                seq_new_len++;

        seq_t *seq_new = (seq_t *)malloc(seq_new_len * sizeof(seq_t));
        size_t pos = 0;
        for (idx_t s = 1; s <= max_sea; ++s)
            if (min_prime[s] != 0)
                seq_new[pos++] = min_prime[s];

        /* ----------  Replace old sequence data  ---------- */
        if (global_seq) free(global_seq);
        global_seq = seq_new;
        global_seq_len = seq_new_len;

        free(min_prime);
        free(primes);
        free(is_prime);

        /* If we still lack enough terms, increase the sieve limit and retry. */
        if (global_seq_len < (size_t)target)
            global_limit <<= 1;  /* double the limit */
    }
}

/* -------------------  Public function  ----------------- */

seq_t a(idx_t n)
{
    /* The sequence is 1‑based, so n must be ≥ 1. */
    if (n == 0)
        return 0;   /* undefined, but prevents a crash */

    ensure_sequence(n);
    return global_seq[n - 1];
}
