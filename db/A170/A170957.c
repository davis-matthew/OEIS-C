/*
 * A170957  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 4.
 * https://oeis.org/A170957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1+x^3)(1+x^7)(1+x^11)(1+x^15)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t mask = 0; mask < (1ULL << 4); ++mask) {
        idx_t sum = 0;
        if (mask & 1ULL) sum += 3;
        if (mask & 2ULL) sum += 7;
        if (mask & 4ULL) sum += 11;
        if (mask & 8ULL) sum += 15;
        if (sum == n) ++count;
    }
    return count;
}
