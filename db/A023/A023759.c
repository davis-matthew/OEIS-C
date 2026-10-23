/*
 * A023759  Nialpdromes: digits in base 3 are in nonincreasing order.
 * https://oeis.org/A023759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = non‑negative integer whose base‑3 digits are in
 *            nonincreasing order. (OEIS A023759)
 * Range:     u8 n <= 52, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;               /* a(1) = 0 */

    /* index among positive terms */
    idx_t k = n - 1;                           /* 0‑based index */

    /* find the length L of the ternary representation */
    idx_t L = 1;
    idx_t cumulative = 0;
    for (;; ++L) {
        idx_t countL = ((L + 2) * (L + 1)) / 2; /* C(L+2,2) */
        countL -= 1;                           /* exclude all‑zero vector */
        if (cumulative + countL >= k) break;
        cumulative += countL;
    }

    /* local index within terms of length L */
    idx_t m = k - cumulative - 1;              /* 0‑based */

    /* Determine counts of 2’s, 1’s and 0’s (t2, t1, t0) */
    idx_t t2 = 0, t1 = 0;

    /* group with leading 1 (no 2’s) */
    idx_t count0 = L;                           /* t1 ∈ [1, L] */
    if (m < count0) {
        t2 = 0;
        t1 = m + 1;
    } else {
        m -= count0;
        for (t2 = 1; t2 <= L; ++t2) {
            idx_t count_t2 = (L - t2) + 1;      /* t1 ∈ [0, L‑t2] */
            if (m < count_t2) {
                t1 = m;
                break;
            }
            m -= count_t2;
        }
    }

    idx_t t0 = L - t2 - t1;

    /* Convert ternary digits to decimal value */
    seq_t v = (seq_t)0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t digit;
        if (i < t2)       digit = 2;
        else if (i < t2 + t1)   digit = 1;
        else                 digit = 0;
        v = v * (seq_t)3 + digit;
    }
    return v;
}
