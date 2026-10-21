/*
 * A056535  Mapping from the ordering by sum to the ordering by product of the
 *          ordered pairs. Inverse permutation to A056534.
 * https://oeis.org/A056535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A056535: Mapping from the ordering by sum to the ordering by product
   (inverse permutation of A056534).
   Algorithm:
     1. Determine the sum S such that the number of ordered pairs (i,j) with
        i+j < S equals n-1.
     2. Within sum S, the pair is (i,j) where i = offset in that sum.
     3. The product d = i*j.
     4. The index in product‑order is 
          sum_{t=1}^{d-1} τ(t)  +  (#divisors of d less than i)  + 1
        where τ(t) is the divisor function.
     5. τ(t) and its prefix sums are pre‑computed up to the current maximum d.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t tau_upto = 0;
static uint64_t *tau_arr = NULL;
static uint64_t *pref_arr = NULL;

/* recompute τ and prefix sums up to 'upper' */
static void compute_tau_pref(size_t upper)
{
    if (tau_upto >= upper)
        return;
    free(tau_arr);
    free(pref_arr);
    tau_arr = (uint64_t*)malloc((upper + 1) * sizeof(uint64_t));
    pref_arr = (uint64_t*)malloc((upper + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= upper; ++i)
        tau_arr[i] = 0;
    for (size_t i = 1; i <= upper; ++i) {
        for (size_t j = i; j <= upper; j += i)
            ++tau_arr[j];
    }
    pref_arr[0] = 0;
    for (size_t i = 1; i <= upper; ++i)
        pref_arr[i] = pref_arr[i-1] + tau_arr[i];
    tau_upto = upper;
}

seq_t a(idx_t n)
{
    /* n==0 is outside the OEIS offset; return 0 (undefined) */
    if (n == 0)
        return (seq_t)0;

    /* find minimal S with (S-1)*S/2 >= n */
    idx_t low = 1, high = 2;
    while ( (unsigned __int128)(high-1) * high / 2 < n )
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (unsigned __int128)(mid-1) * mid / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t S = low;

    /* number of pairs before this sum */
    unsigned __int128 pre128 = (unsigned __int128)(S-2) * (S-1) / 2;
    idx_t pre = (idx_t)pre128;

    idx_t offset = n - pre;          /* 1 .. S-1 */
    idx_t i_val = offset;
    idx_t j_val = S - i_val;
    idx_t d = i_val * j_val;         /* product */

    /* ensure τ and prefix sums up to d */
    compute_tau_pref((size_t)d);

    uint64_t sum_tau_before = pref_arr[d-1];

    /* count divisors of d that are less than i_val */
    size_t rank = 0;
    for (size_t s = 1; (uint64_t)s * s <= d; ++s) {
        if (d % s == 0) {
            if (s < i_val)
                ++rank;
            size_t other = d / s;
            if (other != s && other < i_val)
                ++rank;
        }
    }

    uint64_t result = sum_tau_before + rank + 1;
    return (seq_t)result;
}
