/*
 * A111655  n-th composite number appears n-th composite number times.
 * https://oeis.org/A111655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = the nth composite number that appears that many times.
 *            Let c_k be the k‑th composite (c_1=4, c_2=6, …). Let S_k = c_1 +
 *            c_2 + … + c_k. Then a(n) = c_k where S_{k-1} < n ≤ S_k. The
 *            routine builds composites on demand and keeps the cumulative
 *            sums until the required n is reached
 * Range:     u8 n <= 32, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Arrays of composites and their cumulative sums, kept between
       calls.  They are expanded as needed. */
    static uint64_t *comp = NULL;   /* k-th composite number (uint64_t) */
    static seq_t  *cum  = NULL;     /* cumulative sums as seq_t */
    static size_t size = 0;         /* number of composites stored */
    static size_t cap  = 0;         /* allocated capacity */

    /* Grow arrays if necessary, allocating a new slot for the next
       composite and its cumulative sum. */
    while (1) {
        if (size >= cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            uint64_t *ncomp = (uint64_t *)realloc(comp, newcap * sizeof(uint64_t));
            seq_t    *ncum  = (seq_t     *)realloc(cum,  newcap * sizeof(seq_t));
            if (!ncomp || !ncum) abort();
            comp = ncomp;
            cum  = ncum;
            cap  = newcap;
        }

        /* Generate the next composite number. */
        uint64_t m = (size == 0) ? 4 : comp[size-1] + 1;
        for (;;) {
            bool is_prime = true;
            if (m < 2)           is_prime = false;
            else if (m == 2)     is_prime = false;
            else if (m % 2 == 0) is_prime = (m == 2);
            else {
                for (uint64_t d = 3; d*d <= m; d += 2) {
                    if (m % d == 0) { is_prime = false; break; }
                }
            }
            if (!is_prime) break;
            ++m;
        }

        comp[size] = m;
        seq_t val = (seq_t)m;
        if (size == 0) cum[size] = val;
        else          cum[size] = cum[size-1] + val;
        ++size;

        /* Stop when the cumulative sum passes the requested index n. */
        if (cum[size-1] >= (seq_t)n) break;
    }

    /* Binary search for the first cumulative sum that is ≥ n. */
    size_t lo = 0, hi = size - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum[mid] >= (seq_t)n) hi = mid;
        else                      lo = mid + 1;
    }
    return (seq_t)comp[lo];
}
