/*
 * A035252  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          3.
 * https://oeis.org/A035252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer all of whose prime factors p
 *            with p≡5 or 7 (mod 12) or p=2 occur to an even exponent
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* smallest prime factor table (dynamic) */
static idx_t spf_limit = 0;
static idx_t *spf = NULL;

/* sequence cache */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Build spf array up to 'limit' (inclusive) */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    idx_t *new_spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!new_spf) abort();
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[0] = 0;
    new_spf[1] = 1;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == 0) new_spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (new_spf[i] == 0) new_spf[i] = i;

    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Test if integer n satisfies the exponent parity condition */
static bool allowed(idx_t n)
{
    if (n <= 1) return true;

    while (n > 1) {
        idx_t p = spf[n];
        idx_t e = 0;
        do {
            n /= p;
            ++e;
        } while (n % p == 0);

        /* Special prime 2: exponent must be even   */
        if (p == 2) {
            if (e & 1) return false;
            continue;
        }
        /* Prime 3 is unrestricted */
        if (p == 3) continue;

        /* Other odd primes: determine (3/p) by residue modulo 12 */
        idx_t mod12 = p % 12;
        if (mod12 == 5 || mod12 == 7) {          /* (3/p) = -1 */
            if (e & 1) return false;              /* odd exponent forbidden */
        }
        /* Otherwise exponent arbitrary */
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe */

    /* ensure sieve exists */
    if (!spf) build_spf(65536);    /* initial moderate limit */

    static idx_t candidate = 1;     /* next integer to test */

    /* generate terms until at least n terms are stored */
    while (seq_len < (size_t)n) {
        if (candidate > spf_limit) {
            idx_t new_limit = spf_limit;
            if (new_limit == 0) new_limit = 128;
            while (candidate > new_limit) new_limit *= 2;
            build_spf(new_limit);
        }

        if (allowed(candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!seq) abort();
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return seq[n - 1];
}
