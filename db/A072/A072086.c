/*
 * A072086  Number of steps to reach 1, starting with n and applying the
 *          A072084-map repeatedly.
 * https://oeis.org/A072086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map: for n > 1, next(n) = ∏_{p^e∣n} popcount(p)^e
   Sequence: a(1) = 0; a(n) = 1 + a(next(n))  for n ≥ 2
   (popcount is the number of 1‑bits in the binary representation.) */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1) = 0 */

    idx_t cur = n;
    seq_t steps = 0;

    while (cur != 1) {
        seq_t next = 1;                  /* product of popcount(p) for all prime factors */
        idx_t temp = cur;

        /* trial division up to √temp */
        for (idx_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                seq_t pc = (seq_t)__builtin_popcountll(d);
                /* multiply pc for every occurrence of d */
                while (temp % d == 0) {
                    temp /= d;
                    next *= pc;
                }
            }
        }

        /* remaining prime factor (if any) */
        if (temp > 1) {
            seq_t pc = (seq_t)__builtin_popcountll(temp);
            next *= pc;
        }

        cur = (idx_t)next;              /* next value is guaranteed to fit in idx_t */
        ++steps;
    }

    return steps;
}
