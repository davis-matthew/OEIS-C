/*
 * A036275  The periodic part of the decimal expansion of 1/n. Any initial 0's
 *          are to be placed at end of cycle.
 * https://oeis.org/A036275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) = the periodic part of the decimal expansion of 1/n, with
 * any leading zero digits moved to the end of the cycle.
 * The function simulates the long‑division for 1/n, extracts the repeated
 * fractional part, and returns the integer formed by that cycle after
 * normalising the leading zeros.
 *
 * It works for all n >= 1.  For n of the form 2^i*5^j (i.e. no other
 * prime factors) it returns 0 as specified.
 * Complexity: O(n) time and O(n) memory for n up to a few tens of thousands,
 * which satisfies the required 2‑second limit.
 */
#include <stdlib.h>
#include <stdint.h>

/* assume seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, never invoked */
    
    /* Determine if n has only factors 2 and 5 (terminating decimal) */
    idx_t t = n;
    while ((t & 1) == 0) t >>= 1;        /* remove 2s */
    while (t % 5 == 0) t /= 5;           /* remove 5s */
    if (t == 1) return 0;                /* terminating fraction → no cycle */
    
    /* Allocate arrays for seen remainders and digit storage */
    int *seen = (int *)malloc((n + 1) * sizeof(int));
    uint8_t *digits = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    if (!seen || !digits) return 0;     /* allocation failure safeguard */
    
    /* Initialise seen[rem] to -1 (unseen) */
    for (idx_t i = 0; i <= n; ++i) seen[i] = -1;
    
    idx_t pos = 0;
    idx_t rem = 1 % n;                  /* initial remainder (n > 1) */
    
    /* Long division simulation until a remainder repeats */
    while (seen[rem] == -1) {
        seen[rem] = (int)pos;           /* record first occurrence */
        rem *= 10;
        uint8_t digit = (uint8_t)(rem / n);
        rem %= n;
        digits[pos++] = digit;          /* store digit of the decimal expansion */
    }
    
    idx_t cycle_start = (idx_t)seen[rem];   /* start index of the cycle */
    idx_t L = pos - cycle_start;            /* length of the cycle */
    
    /* Find the first non‑zero digit within the cycle (for possible rotation) */
    idx_t first_nonzero = cycle_start;
    while (first_nonzero < pos && digits[first_nonzero] == 0)
        ++first_nonzero;
    
    /* Assemble the cycle integer, rotating leading zeros to the end */
    seq_t result = 0;
    for (idx_t i = first_nonzero; i < pos; ++i)
        result = result * 10 + digits[i];
    for (idx_t i = cycle_start; i < first_nonzero; ++i)
        result = result * 10 + digits[i];
    
    free(seen);
    free(digits);
    return result;
}
