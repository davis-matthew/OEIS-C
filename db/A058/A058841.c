/*
 * A058841  From Renyi's "beta expansion of 1 in base 3/2": sequence gives
 *          lengths of runs of 0's in A058840.
 * https://oeis.org/A058841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058841: a(n) = length of the n‑th run of consecutive 0's
 * in the greedy β–expansion of 1 for β = 3/2.  The algorithm
 * simulates the β–expansion and records zero‑run lengths as they
 * appear.  It uses a lightweight big‑integer representation that
 * stores the current numerator as an array of 32‑bit limbs.  The
 * routine runs in O(n) time and O(log n) memory. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 by convention. */
    if (n == 0)
        return (seq_t)0;

    /* ------------------------------------------------------------------ */
    /* Local big integer type:  little‑endian array of 32‑bit limbs.      */
    struct BigInt {
        uint32_t *limbs;   /* array of words, least significant first   */
        size_t   len;      /* number of used words                       */
    };

    /* ------------------------------------------------------------------ */
    /* Initialise the rational r = 1 / 2^exp  (exp = 0 initially).       */
    struct BigInt R;
    R.len = 1;
    R.limbs = (uint32_t*)malloc(sizeof(uint32_t));
    if (!R.limbs) abort();
    R.limbs[0] = 1;          /* numerator = 1, denominator = 1   */
    uint32_t exp = 0;        /* current exponent of denominator   */

    /* ------------------------------------------------------------------ */
    /* Run‑length generation variables.                                 */
    uint64_t zero_len = 0;   /* current length of the zero run     */
    idx_t    run_cnt  = 0;   /* number of zero runs recorded       */
    int      first_one = 1;  /* flag for the very first '1'        */

    /* ------------------------------------------------------------------ */
    /* Generate digits until the desired run (index n) is recorded.      */
    while (1) {
        /* ---- 1. Multiply the numerator by 3.                          */
        uint64_t carry = 0;
        for (size_t i = 0; i < R.len; ++i) {
            uint64_t t = (uint64_t)R.limbs[i] * 3 + carry;
            R.limbs[i] = (uint32_t)(t & 0xFFFFFFFF);
            carry     = t >> 32;
        }
        if (carry) {
            R.limbs = (uint32_t*)realloc(R.limbs,
                                         sizeof(uint32_t) * (R.len + 1));
            if (!R.limbs) abort();
            R.limbs[R.len++] = (uint32_t)carry;
        }

        /* ---- 2. New denominator exponent and the digit to output.     */
        uint32_t shift = exp + 1;                     /* exp -> exp+1   */
        uint32_t widx  = shift >> 5;                  /* divide by 32   */
        uint32_t bit   = shift & 0x1F;                /* mod 32         */
        uint32_t d = 0;

        if (widx < R.len)
            d = (R.limbs[widx] >> bit) & 1U;          /* current digit  */

        if (d) {                                     /* digit is 1     */
            /* clear the bit so that future multiplications trigger   */
            R.limbs[widx] &= ~(1U << bit);
            while (R.len > 1 && R.limbs[R.len - 1] == 0)
                --R.len;
        }

        exp = shift;                                  /* advance exponent */

        /* ---- 3. Update zero run counters.                          */
        if (d == 0) {
            ++zero_len;
        } else { /* d == 1 */
            if (!first_one) {                         /* record a run   */
                ++run_cnt;
                if (run_cnt == n) {
                    free(R.limbs);
                    return (seq_t)zero_len;
                }
            }
            first_one = 0;
            zero_len = 0;
        }
    }
}
