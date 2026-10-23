/*
 * A023045  8th differences of factorial numbers.
 * https://oeis.org/A023045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 28*n^7 + 350*n^6 + 2492*n^5 + 10899*n^4 +
 *            29596*n^3 + 48082*n^2 + 42048*n + 14833) * n!
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stddef.h> /* for size_t if needed in other utilities */

seq_t a(idx_t n) {
    /* Keep a running factorial to avoid recomputing from scratch. */
    static idx_t fact_n = 0;
    static seq_t fact = 1;

    if (n > fact_n) {
        for (idx_t i = fact_n + 1; i <= n; ++i) {
            fact = fact * (seq_t)i;
        }
        fact_n = n;
    }

    seq_t sn   = (seq_t)n;
    seq_t poly = (seq_t)1;

    poly = poly * sn + (seq_t)28;
    poly = poly * sn + (seq_t)350;
    poly = poly * sn + (seq_t)2492;
    poly = poly * sn + (seq_t)10899;
    poly = poly * sn + (seq_t)29596;
    poly = poly * sn + (seq_t)48082;
    poly = poly * sn + (seq_t)42048;
    poly = poly * sn + (seq_t)14833;

    return poly * fact;
}
