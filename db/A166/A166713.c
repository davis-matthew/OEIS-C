/*
 * A166713  Alliterative-digit numbers: Positive integers n such that the
 *          English names of the decimal digits of n begin with the same
 *          letter; ignore single-digit numbers.
 * https://oeis.org/A166713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating digit strings of the same first-letter class. */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t L = 2;                       /* length of numbers being counted */

    /* Find the length L for which the nth term lies. */
    while (1) {
        idx_t pow2L = (idx_t)1 << L;
        idx_t per_len = 3 * pow2L + 3; /* rep1 + 2^L t + 2^L f + 2^L s + rep8 + rep9 */
        if (cumulative + per_len >= n) break;
        cumulative += per_len;
        ++L;
    }

    idx_t offset  = n - cumulative;     /* 1‑based index within length L */
    idx_t offset0 = offset - 1;          /* 0‑based index */
    idx_t pow2L   = (idx_t)1 << L;

    seq_t val = 0;

    if (offset0 == 0) {                      /* repdigit 1 */
        for (int i = 0; i < (int)L; ++i) {
            val = val * 10 + 1;
        }
    } else if (offset0 <= pow2L) {           /* class t: digits 2 and 3 */
        idx_t rank = offset0 - 1;
        for (int i = (int)L - 1; i >= 0; --i) {
            int bit   = (rank >> i) & 1;
            int digit = 2 + bit;
            val = val * 10 + digit;
        }
    } else if (offset0 <= 2 * pow2L) {       /* class f: digits 4 and 5 */
        idx_t rank = offset0 - pow2L - 1;
        for (int i = (int)L - 1; i >= 0; --i) {
            int bit   = (rank >> i) & 1;
            int digit = 4 + bit;
            val = val * 10 + digit;
        }
    } else if (offset0 <= 3 * pow2L) {       /* class s: digits 6 and 7 */
        idx_t rank = offset0 - 2 * pow2L - 1;
        for (int i = (int)L - 1; i >= 0; --i) {
            int bit   = (rank >> i) & 1;
            int digit = 6 + bit;
            val = val * 10 + digit;
        }
    } else if (offset0 == 3 * pow2L + 1) {   /* repdigit 8 */
        for (int i = 0; i < (int)L; ++i) {
            val = val * 10 + 8;
        }
    } else {                                 /* repdigit 9 */
        for (int i = 0; i < (int)L; ++i) {
            val = val * 10 + 9;
        }
    }

    return val;
}
