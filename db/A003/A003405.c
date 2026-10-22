/*
 * A003405  G.f.: (1 + x^4 + x^7 + 2*x^8 + x^9 + x^12 + x^16)/Product_{i=1..8}
 *          (1 - x^i).
 * https://oeis.org/A003405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1 + x^4 + x^7 + 2*x^8 + x^9 + x^12 +
 *            x^16)/Product_{i=1..8} (1 - x^i); computed by the linear
 *            recurrence from the expanded denominator 1 - x - x^2 + x^5 + x^7
 *            + x^9 - x^11 - 2*x^12 - x^13 - x^15 + x^16 + x^17 + 2*x^18 +
 *            x^19 + x^20 - x^21 - x^23 - 2*x^24 - x^25 + x^27 + x^29 + x^31 -
 *            x^34...
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 261, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int A3405_D[37] = {
    1, -1, -1, 0, 0, 1, 0, 1, 0, 1, 0, -1, -2, -1, 0, -1, 1, 1, 2, 1, 1,
    -1, 0, -1, -2, -1, 0, 1, 0, 1, 0, 1, 0, 0, -1, -1, 1
};

static const int A3405_N[17] = {
    1, 0, 0, 0, 1, 0, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0, 1
};

seq_t a(idx_t n) {
    seq_t buf[37];
    for (int i = 0; i < 37; i++) buf[i] = 0;

    for (idx_t m = 0; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        if (m <= 16) {
            for (int t = 0; t < A3405_N[m]; t++) pos += 1;
        }
        idx_t kmax = m < 36 ? m : 36;
        for (idx_t k = 1; k <= kmax; k++) {
            int c = A3405_D[k];
            if (c == 0) continue;
            idx_t j = (m - k) % 37;
            if (c < 0) {
                for (int t = 0; t < -c; t++) pos += buf[j];
            } else {
                for (int t = 0; t < c; t++) neg += buf[j];
            }
        }
        buf[m % 37] = pos - neg;
    }
    return buf[n % 37];
}
