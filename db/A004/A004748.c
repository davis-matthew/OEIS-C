/*
 * A004748  Binary expansion contains 101.
 * https://oeis.org/A004748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 77 terms through n = 7962 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n-th positive integer whose binary representation contains the substring '101' */

static int contains_101(seq_t num) {
    if (num < 5) return 0;
    
    while (num >= 5) {
        if ((num & 7) == 5) {
            return 1;
        }
        num >>= 1;
    }
    return 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 0;
    
    while (count < n) {
        candidate++;
        if (contains_101(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
