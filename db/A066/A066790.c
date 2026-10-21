/*
 * A066790  a(n) = least natural number k such that the distance between (n,
 *          sigma(n)) and (n+k, sigma(n+k)) is an integer (i.e., k^2 +
 *          (sigma(n+k) - sigma(n))^2 is a square), if such k exists; 0
 *          otherwise.
 * https://oeis.org/A066790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   isqrt_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A066790:  a(n) = least k>0 with  k² + (σ(n+k)-σ(n))² a perfect square.
   σ(m) is the sum of divisors of m.  If no such k exists (never happens in
   known data) the function returns 0.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/isqrt_seq.h"

/* Cache of σ(i) values.  Index 0 holds 0 for convenience. */
static seq_t *sigma_cache = NULL;   /* array of σ(i) */
static size_t   sigma_len  = 0;      /* first unused index in cache   */
static size_t   sigma_cap  = 0;      /* current capacity */

/* Ensure sigma_cache contains σ values for 0 .. up_to inclusive. */
static void ensure_sigma(size_t up_to)
{
    if (up_to < sigma_len) return;          /* already present */

    /* Grow the cache capacity if necessary. */
    if (up_to + 1 > sigma_cap) {
        size_t new_cap = sigma_cap ? sigma_cap << 1 : 16;
        if (new_cap <= up_to + 1) new_cap = up_to + 1;
        seq_t *tmp = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                    /* allocation failed – caller will bail */
        sigma_cache = tmp;
        sigma_cap   = new_cap;
    }

    /* Compute σ(i) for new indices. */
    for (size_t i = sigma_len; i <= up_to; ++i) {
        if (i == 0) {
            sigma_cache[i] = (seq_t)0;
        } else {
            sigma_cache[i] = sigma_of((idx_t)i);
        }
    }
    sigma_len = up_to + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Ensure cache contains σ(n). */
    ensure_sigma((size_t)n);
    seq_t sigma_n = sigma_cache[(size_t)n];

    for (idx_t k = 1; ; ++k) {
        idx_t nk = n + k;
        ensure_sigma((size_t)nk);
        seq_t sigma_k = sigma_cache[(size_t)nk];

        seq_t diff = sigma_k > sigma_n ? sigma_k - sigma_n : sigma_n - sigma_k;
        seq_t kseq = (seq_t)k;
        seq_t val  = kseq * kseq + diff * diff;
        seq_t sq   = isqrt_seq(val);

        if (sq * sq == val) return kseq;   /* found minimal k */
    }
}
