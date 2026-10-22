/*
 * A000867  Numbers beginning with letter 'f' in English.
 * https://oeis.org/A000867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   a number is a term iff the name of its leading 3-digit group
 *            (the part before "thousand"/"million"/...) begins with 'f', i.e.
 *            that group is one of 4,5,14,15,40..59,400..599 (224 values). So
 *            the terms with k groups form 224*1000^(k-1) consecutive blocks:
 *            a(n) = g*1000^(k-1) + r, where g is the lea...
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lead_group
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lead_group.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)n - 1;
    seq_t block = (seq_t)1;
    while (m >= block * 224) {
        m -= block * 224;
        block *= 1000;
    }
    long long i = (long long)(m / block);
    return lead_group(i) * block + m % block;
}
