/*
 * A099652  Largest number arising if f[x]=1+A003132(x) function is iterated
 *          until steady state reached. Compare with A099648.
 * https://oeis.org/A099652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = max of the orbit of n under f(x) = 1 + A003132(x),
 *            listing the start value and iterating until a value repeats
 *            (iteration, not a closed form)
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A003132_a(seq_t num) {
    seq_t result = 0;
    while (num > 0) {
        seq_t digit = num % 10;
        result += digit * digit;
        num /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    bool visited[4096];
    for (int i = 0; i < 4096; i++) visited[i] = false;
    seq_t cur = (seq_t)n;
    seq_t mx = cur;
    for (;;) {
        if (cur < 4096) {
            int k = (int)(long long)cur;
            if (visited[k]) break;
            visited[k] = true;
        }
        if (cur > mx) mx = cur;
        cur = (seq_t)1 + A003132_a(cur);
    }
    return mx;
}
