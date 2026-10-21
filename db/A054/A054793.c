/*
 * A054793  Earliest sequence with a(a(n)) = n^4.
 * https://oeis.org/A054793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   if n is a 4th power: a(n) = a(n^(1/4))^4 else let d = n –
 *            (floor(n^(1/4)))^4 if d is odd: a(n) = n + 1 else: a(n) = (n –
 *            1)^4 Base cases: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   fourth_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fourth_root_floor.h"

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t k  = fourth_root_floor(nn);
    unsigned __int128 k4 = (unsigned __int128)k * k * k * k;

    if (k4 == nn) {                /* n is a perfect 4th power */
        seq_t sub = a((idx_t)k);
        seq_t sq  = sub * sub;
        return sq * sq;
    } else {                        /* n is not a 4th power */
        uint64_t diff = nn - (uint64_t)k4;
        if (diff & 1ULL) {          /* odd difference */
            return (seq_t)nn + 1;
        } else {                    /* even difference */
            seq_t base = (seq_t)(nn - 1);
            seq_t sq   = base * base;
            return sq * sq;
        }
    }
}
