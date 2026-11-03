/*
 * A098378  Number of characters needed to write number n in the traditional
 *          Ethiopic (Geez) number system.
 * https://oeis.org/A098378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of Ethiopic characters needed to write number n in
 *            the traditional Ethiopic (Geez) number system
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t ethiopic_multiplier_count(idx_t k) {
    /* Characters needed for the multiplier of 100^k
       k = 1 → ፻ (100)            → 1 character
       k = 2 → ፼ (10 000)          → 1 character
       k ≥ 3 → k copies of ፻      → k characters */
    if (k == 1) return 1;     /* ፻   */
    if (k == 2) return 1;     /* ፼   */
    return (seq_t)k;          /* k copies of ፻ */
}

static inline seq_t ethiopic_unit_count(idx_t d) {
    /* Characters needed to represent a number 0 < d < 100 in base-100
       (tens digit + optional units digit) */
    if (d == 0) return 0;
    seq_t c = 0;
    if (d >= 10) c++;        /* tens character (10,20,…,90) */
    if (d % 10 != 0) c++;    /* units character (1–9) */
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;     /* offset starts at 1, but harmless */
    seq_t cnt = 0;
    idx_t k = 0;
    idx_t rem = n;
    while (rem > 0) {
        idx_t d = rem % 100;          /* current 100‑block */
        if (k == 0) {                 /* units block */
            cnt += ethiopic_unit_count(d);
        } else {                      /* higher blocks */
            if (d != 0) {
                if (d == 1) {
                    /* block value 1 -> only the multiplier character */
                    cnt += ethiopic_multiplier_count(k);
                } else {
                    /* block value >1 -> units + multiplier */
                    cnt += ethiopic_unit_count(d);
                    cnt += ethiopic_multiplier_count(k);
                }
            }
        }
        rem /= 100;
        k++;
    }
    return cnt;
}
