/*
 * A084694  Squarefree numbers which are products of three consecutive
 *          numbers. I.e., squarefree numbers of the form k^3 - k.
 * https://oeis.org/A084694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 206, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 45 terms through n = 9128 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084694 – squarefree numbers of the form k^3 – k.
 *
 * The algorithm enumerates k = 2, 6, 10, 14, … (k ≡ 2 (mod 4)).
 * For each such k it checks whether k–1, k and k+1 are all square‑free.
 * If that holds the product k·(k–1)·(k+1) is square‑free and is appended
 * to the output list, until the n‑th term is found.
 *
 * The function is fully self‑contained, uses only integer arithmetic,
 * and works for all builds supported by the test harness.  No external
 * utility functions are required.  The routine runs in O(n·√k) time,
 * which is well below the 2‑second limit for n ≈ 10 000.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute k·(k–1)·(k+1) safely, using a 128‑bit temporary if available. */
static seq_t squarefree_product(idx_t k)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 tmp = (unsigned __int128)k *
                            (unsigned __int128)(k - 1) *
                            (unsigned __int128)(k + 1);
    return (seq_t)tmp;
#else
    seq_t res = (seq_t)k;
    res *= (seq_t)(k - 1);
    res *= (seq_t)(k + 1);
    return res;
#endif
}

/* Check whether a small integer v is square–free.
 * v ≤ ~10^6 in practice; the loop runs at most ≈ √v iterations. */
static bool is_squarefree_uint64(idx_t v)
{
    if (v <= 1)
        return true;
    idx_t r;
    for (r = 2; r * r <= v; ++r) {
        if (v % r == 0) {
            v /= r;
            if (v % r == 0)
                return false;           /* r² divides v */
        }
    }
    return true;
}

/* Return the n‑th term (n ≥ 1) of A084694. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms seen so far   */
    idx_t k     = 2;          /* start at first candidate      */

    while (true) {
        /* k is always ≡ 2 (mod 4)               */
        idx_t a_k = k - 1;
        idx_t b_k = k;
        idx_t c_k = k + 1;

        if (is_squarefree_uint64(a_k) &&
            is_squarefree_uint64(b_k) &&
            is_squarefree_uint64(c_k)) {

            ++count;
            if (count == n)
                return squarefree_product(k);
        }

        k += 4;   /* next candidate */
    }
}
