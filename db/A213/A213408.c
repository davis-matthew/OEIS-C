/*
 * A213408  Sequence A002654 with the zero terms omitted.
 * https://oeis.org/A213408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002654(k) where k is the n-th index with A002654(k) != 0
 *            (A002654(k) = number of divisors of k that are 1 mod 4 minus
 *            number that are 3 mod 4); found by scanning k upward
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Validated: partial, pass, 113 terms through n = 113 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A002654_a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;

    while (temp % 2 == 0) {
        temp /= 2;
    }

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exponent = 0;
            while (temp % p == 0) {
                exponent++;
                temp /= p;
            }
            if (p % 4 == 1) {
                result *= (seq_t)(exponent + 1);
            } else {
                if (exponent % 2 == 1) {
                    return 0;
                }
            }
        }
    }

    if (temp > 1) {
        if (temp % 4 == 1) {
            result *= 2;
        } else {
            return 0;
        }
    }

    return result;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1;; k++) {
        seq_t v = A002654_a(k);
        if (v != 0) {
            count++;
            if (count == n) {
                return v;
            }
        }
    }
}
