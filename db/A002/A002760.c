/*
 * A002760  Squares and cubes.
 * https://oeis.org/A002760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   merge of the squares k^2 and the cubes k^3 in increasing order,
 *            each common value (sixth power) counted once; a(1) = 0
 * Range:     u8 n <= 20, u16 n <= 290, u32 n <= 10444, u64 n <= 10444,
 *            u128 n <= 10444, big any n
 * Validated: standard, pass, 69 terms through n = 9270 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int pw_ok(seq_t b, int k, seq_t *out) {
    seq_t r = 1;
    int t;
    for (t = 0; t < k; t++) {
        seq_t p = r * b;
        if (b != 0 && p / b != r) return 0;
        r = p;
    }
    *out = r;
    return 1;
}

seq_t a(idx_t n) {
    seq_t i = 0, j = 0, s = 0, c = 0;
    int sv = pw_ok(i, 2, &s);
    int cv = pw_ok(j, 3, &c);
    idx_t k;
    for (k = 1;; k++) {
        seq_t v;
        if (!sv && !cv) return 0;
        if (sv && cv && s == c) {
            v = s;
            i++; sv = pw_ok(i, 2, &s);
            j++; cv = pw_ok(j, 3, &c);
        } else if (sv && (!cv || s < c)) {
            v = s;
            i++; sv = pw_ok(i, 2, &s);
        } else {
            v = c;
            j++; cv = pw_ok(j, 3, &c);
        }
        if (k == n) return v;
    }
}
