/*
 * A045112  Numbers whose base-4 representation contains no 1's and exactly
 *          three 3's.
 * https://oeis.org/A045112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045112: numbers whose base‑4 representation contains no 1's
   and exactly three 3's.
   The function generates the sequence by brute‑force scanning of natural
   numbers, checking the base‑4 digit conditions.  The first term is 63
   (base‑4 333); numbers grow slowly enough that collecting up to 10 000
   terms requires scanning fewer than a million integers, which fits easily
   in the 2‑second time limit.
*/
#include <stdbool.h>

static inline bool has_valid_base4_digits(seq_t v)
/* Returns true iff v has no digit 1 in base‑4 and exactly three digits 3. */
{
    seq_t temp = v;
    int count3 = 0;

    while (temp != (seq_t)0) {
        uint64_t d = (uint64_t)(temp % (seq_t)4);  /* digit 0–3 */
        if (d == (uint64_t)1)          /* digit 1 disallowed */
            return false;
        if (d == (uint64_t)3) {          /* count 3's */
            count3++;
            if (count3 > 3)              /* more than three 3's */
                return false;
        }
        temp /= (seq_t)4;
    }
    return (count3 == 3);                  /* must have exactly 3 of them */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; n=0 not used */
    seq_t candidate = (seq_t)0;
    idx_t found = 0;

    while (1) {
        if (has_valid_base4_digits(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
