/*
 * A101217  Base-4 representation of positive integers whose base-2 run-length
 *          representation is the same as their base-4 representation.
 * https://oeis.org/A101217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101217: base‑4 representation of the positive integers whose
   binary run‑length encoding equals that base‑4 representation.
   Construction:
     a(1) = 1.
     For even n ≥ 2: digits = 1,1,3 followed by (1,3) repeated (n/2 – 1) times.
     For odd n ≥ 3: digits = 3,2,1 followed by (1,3) repeated ((n–3)/2) times.
   The function returns the integer whose decimal digits are exactly these
   base‑4 digits. */
seq_t a(idx_t n)
{
    /* n = 0 is outside the defined range – return 0 safely */
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t val = 0;

    if ((n & 1) == 0) {          /* even n */
        val = val * 10 + 1;      /* first digit 1 */
        val = val * 10 + 1;      /* second digit 1 */
        val = val * 10 + 3;      /* third digit 3 */
        idx_t reps = (n / 2) - 1;/* number of (1,3) pairs */
        for (idx_t i = 0; i < reps; ++i) {
            val = val * 10 + 1;
            val = val * 10 + 3;
        }
    } else {                     /* odd n ≥ 3 */
        val = val * 10 + 3;      /* first digit 3 */
        val = val * 10 + 2;      /* second digit 2 */
        val = val * 10 + 1;      /* third digit 1 */
        idx_t reps = (n - 3) / 2;/* number of (1,3) pairs */
        for (idx_t i = 0; i < reps; ++i) {
            val = val * 10 + 1;
            val = val * 10 + 3;
        }
    }

    return val;
}
