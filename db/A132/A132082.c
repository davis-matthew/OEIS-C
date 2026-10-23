/*
 * A132082  Triangle read by rows: row n contains the n smallest positive
 *          integers which are coprime to (2^n +1).
 * https://oeis.org/A132082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the k‑th smallest positive integer that is coprime to
 *            2^r+1, * where r is the smallest integer such that r(r+1)/2 ≥ n,
 *            and * k = n – r(r–1)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A132082. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1, but 0 is unused */

    /* Find r such that r(r+1)/2 ≥ n (binary search with overflow protection). */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k = n - r(r-1)/2  (position within row r, 1‑based) */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;

    /* Search for the k‑th coprime integer. */
    idx_t candidate = 1;
    while (1) {
        seq_t mod = mod_pow((seq_t)2, (uint64_t)r, (seq_t)candidate);
        mod = (mod + (seq_t)1) % (seq_t)candidate;   /* (2^r + 1) mod candidate */
        seq_t g = gcd((seq_t)candidate, mod);        /* gcd(candidate, 2^r + 1) */
        if (g == (seq_t)1) {
            if (--k == 0)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
