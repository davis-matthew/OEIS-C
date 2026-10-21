/*
 * A205960  Smallest odd number with digit sum equal to n.
 * https://oeis.org/A205960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest odd number with digit sum equal to n.
 *            Implementation: greedy construction of the minimal decimal
 *            number with the required sum and last digit odd
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 84, u64 n <= 171,
 *            u128 n <= 344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)n;           /* digit sum to achieve */
    idx_t L = (n + 8) / 9;          /* initial length (ceil(n/9)) */

    for (;; ++L) {
        if (n < 1 || n > 9 * L) continue;   /* length too short or too long */

        seq_t result = 0;
        seq_t remaining = sum;
        int ok = 1;                      /* flag for feasible construction */

        for (idx_t i = 0; i < L; ++i) {
            seq_t digit = 0;

            if (i == L - 1) {                /* last position */
                if (remaining >= 1 && remaining <= 9 &&
                    (remaining & 1)) {
                    digit = remaining;
                } else {
                    ok = 0; break;
                }
            } else {                       /* intermediate positions */
                seq_t min_d = (i == 0) ? 1 : 0;
                int found = 0;

                for (int d = (int)min_d; d <= 9; ++d) {
                    if (remaining < (seq_t)d) break;

                    seq_t rem = remaining - (seq_t)d;
                    if (rem < 1) break;                         /* cannot leave 0 for the odd final digit */

                    idx_t remPos = L - i - 1;
                    seq_t maxRem = (seq_t)(9 * remPos);
                    if (rem <= maxRem) {
                        digit = (seq_t)d;
                        remaining = rem;
                        found = 1;
                        break;
                    }
                }
                if (!found) { ok = 0; break; }
            }

            result = result * 10 + digit;
        }

        if (ok) return result;          /* found the minimal odd number */
    }
}
