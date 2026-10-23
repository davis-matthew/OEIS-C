/*
 * A039343  Numbers whose base-8 representation has the same nonzero number of
 *          2's and 4's.
 * https://oeis.org/A039343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search of base‑8 digits for equal number of 2's and 4's */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand   = 0;
    const seq_t eight = (seq_t)8;

    while (found < n) {
        seq_t temp = cand;
        unsigned int cnt2 = 0, cnt4 = 0;

        /* Count digits 2 and 4 in base 8 representation */
        do {
            unsigned int digit = (unsigned int)(temp % eight);
            if (digit == 2u) ++cnt2;
            else if (digit == 4u) ++cnt4;
            temp /= eight;
        } while (temp > (seq_t)0);

        if (cnt2 > 0 && cnt2 == cnt4) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }

    /* Should never reach here; return 0 as a safe fallback */
    return (seq_t)0;
}
