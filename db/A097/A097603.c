/*
 * A097603  Multiples of perfect numbers.
 * https://oeis.org/A097603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097603: Multiples of perfect numbers
   a(n) is the nth smallest positive integer that is divisible by at
   least one perfect number (6, 28, 496, 8128, 33550336, …).  The
   implementation simply scans the positive integers, counts those
   with a perfect‑number divisor, and returns the nth such integer.
   It works for any seq_t and idx_t as defined by the build. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for 0, safe return */

    /* Known perfect numbers. All fit in 64 bits and therefore can
       be safely cast to seq_t regardless of its size. */
    static const seq_t perfect[8] = {
        6ULL,
        28ULL,
        496ULL,
        8128ULL,
        33550336ULL,
        8589869056ULL,
        137438691328ULL,
        2305843008139952128ULL
    };
    const idx_t P = 8;                       /* number of perfect numbers */

    idx_t found = 0;                         /* how many terms found so far */
    idx_t x = 1;                              /* candidate integer */

    while (found < n) {
        seq_t val = (seq_t)x;

        /* Check divisibility by at least one perfect number. */
        for (idx_t i = 0; i < P; ++i) {
            if (val % perfect[i] == 0) {
                ++found;
                break;
            }
        }
        if (found == n) return val;
        ++x;
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)x;
}
