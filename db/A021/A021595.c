/*
 * A021595  Decimal expansion of 1/591.
 * https://oeis.org/A021595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A021595: Decimal expansion of 1/591.
 * a(n) is the nth digit after the decimal point of 1/591,
 * 0‑based (so a(0) is the first digit shown in the OEIS entry).
 *
 * Formula: a(n) = digit at position n in the long‑division of 1 by 591
 * (i.e., the nth remainder‑division step of 1/591).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder is always < 591, so a 64‑bit integer is safely enough. */
    uint64_t remainder = 1;      /* numerator of 1/591 */

    /* Long division: for i = 0,1,2,... produce digit a(i). We need the
       (n+1)‑th iteration to obtain a(n). */
    for (idx_t i = 0; ; ++i) {
        remainder *= 10ULL;            /* bring down next decimal digit */
        uint64_t digit = remainder / 591ULL;
        remainder     = remainder % 591ULL;

        if (i == n) {
            return (seq_t)digit;        /* exact conversion: digit 0‑9 */
        }
        /* i == n will always terminate, as n is finite. */
    }
}
