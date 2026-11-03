/*
 * A005767  Solutions k to k^2 = a^2 + b^2 + c^2 (a,b,c > 0).
 * https://oeis.org/A005767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not of the form 2^j or 5*2^j
 *            (Fraser and Gordon); found by counting excluded numbers up to k
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8791 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_form(seq_t start, seq_t k) {
    seq_t count = 0;
    seq_t p = start;
    while (p <= k) {
        count++;
        if (p > k / 2) break;
        p *= 2;
    }
    return count;
}

static seq_t count_valid(seq_t k) {
    return k - count_form(1, k) - count_form(5, k);
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t k = target;
    /* count_valid is nondecreasing; a(n) = n + O(log n), so this is short */
    while (count_valid(k) < target) k++;
    return k;
}
