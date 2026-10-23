/*
 * A161895  Write down binary n as a string of 0's and 1's. Consider the runs
 *          of 1's (bounded by 0's or by the edge of the string) alternating
 *          with the runs of 0's (bounded by 1's or by the edge of the string)
 *          in the string. Then, a(n) = the number of positive binary integers
 *          that contain the same lengths of...
 * https://oeis.org/A161895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes OEIS sequence A161895.
   For an integer n≥1, let its binary representation start with a run of 1's,
   then alternating runs of 0's and 1's.  
   If the lengths of runs of 1's are (l1,…,lk) and the lengths of runs of 0's are
   (m1,…,mp) (order of runs is fixed), then
     a(n) = (k! / ∏(s! for each multiplicity s of a 1‑run length))
            × (p! / ∏(t! for each multiplicity t of a 0‑run length)).
   The function counts all distinct binary integers that have exactly the same
   multiset of run‑lengths for 1's and 0's, which equals the above product.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- internal factorial cache ---------- */
static seq_t fact_cache[65];
static bool fact_ready = false;

static void init_fact(void)
{
    if (!fact_ready) {
        fact_cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 64; ++i)
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        fact_ready = true;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 */
    init_fact();

    /* arrays to count multiplicities of run lengths (1..64) */
    unsigned count1[65] = {0};
    unsigned count0[65] = {0};
    unsigned total1 = 0, total0 = 0;

    /* ----- find most significant bit ----- */
    /* use builtin if available, otherwise fall back to loop */
    idx_t msb = 0;
#if defined(__GNUC__) || defined(__clang__)
    msb = 63 - __builtin_clzll(n);
#else
    idx_t tmp = n;
    while (tmp >>= 1) ++msb;
#endif

    /* ----- iterate bits from msb to 0 ----- */
    idx_t bit = (n >> msb) & 1;
    idx_t len = 1;

    for (idx_t i = msb; i-- > 0; ) {
        idx_t next_bit = (n >> i) & 1;
        if (next_bit == bit) {
            ++len;
        } else {
            /* store completed run */
            if (bit == 1) {
                ++count1[(size_t)len];
                ++total1;
            } else {
                ++count0[(size_t)len];
                ++total0;
            }
            bit = next_bit;
            len = 1;
        }
    }
    /* store the last run */
    if (bit == 1) {
        ++count1[(size_t)len];
        ++total1;
    } else {
        ++count0[(size_t)len];
        ++total0;
    }

    /* ----- compute permutations for 1‑runs ----- */
    seq_t res1 = fact_cache[total1];
    for (size_t l = 1; l <= 64; ++l) {
        unsigned c = count1[l];
        if (c > 1)
            res1 /= fact_cache[c];
    }

    /* ----- compute permutations for 0‑runs ----- */
    seq_t res0 = fact_cache[total0];
    for (size_t l = 1; l <= 64; ++l) {
        unsigned c = count0[l];
        if (c > 1)
            res0 /= fact_cache[c];
    }

    /* final answer */
    return res1 * res0;
}
