/*
 * A072906  Least k >= 1 such that floor(n/k) is squarefree.
 * https://oeis.org/A072906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k ≥ 1 such that floor(n / k) is square‑free
 * Range:     u8 n <= 8971, u16 n <= 8971, u32 n <= 8971, u64 n <= 8971,
 *            u128 n <= 8971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* square‑free flag array, 1‑based.  is_sf[i] == true   ⇔   i is square‑free. */
static bool *is_sf = NULL;
static idx_t sf_max = 0;   /* largest index computed so far */

/* Ensure square‑free data up to 'limit' is available. */
static void compute_sf(idx_t limit)
{
    if (limit <= sf_max) return;          /* already computed */

    /* Allocate new array.  Cast required for C++ compatibility. */
    bool *new_arr = (bool *)calloc(limit + 1, sizeof(bool));
    if (!new_arr) abort();                /* fatal on allocation failure */

    /* Initially assume everything is square‑free. */
    for (idx_t i = 1; i <= limit; ++i) new_arr[i] = true;

    /* Mark non‑square‑free numbers: multiples of p^2. */
    for (idx_t p = 2; p <= limit / p; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq)
            new_arr[j] = false;
    }

    /* Free previously allocated array and store new one. */
    free(is_sf);
    is_sf = new_arr;
    sf_max = limit;
}

seq_t a(idx_t n)
{
    /* Compute square‑free info up to n. */
    compute_sf(n);

    /* Find the minimal k such that floor(n/k) is square‑free. */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;                 /* floor division */
        if (is_sf[m]) return (seq_t)k;
    }

    /* k == n always works because n/n == 1 is square‑free. */
    return (seq_t)n;
}
