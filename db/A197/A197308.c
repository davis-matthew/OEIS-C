/*
 * A197308  Divisors of 11111111.
 * https://oeis.org/A197308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th divisor of 11111111 in increasing order
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Build all divisors of the fixed number 11111111 = 11 * 73 * 137 * 101. */
static void build_divisors(seq_t *out, size_t *cnt) {
    const uint64_t primes[4] = {11ULL, 73ULL, 137ULL, 101ULL};
    const size_t num_primes = 4;
    const size_t total = 1ULL << num_primes;      /* 2^4 = 16 */

    size_t idx = 0;
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t prod = 1;
        for (size_t p = 0; p < num_primes; ++p) {
            if (mask & (1ULL << p))
                prod *= (seq_t)primes[p];
        }
        out[idx++] = prod;
    }

    /* Simple insertion sort – list is tiny (16). */
    for (size_t i = 1; i < total; ++i) {
        seq_t key = out[i];
        size_t j = i;
        while (j > 0 && out[j - 1] > key) {
            out[j] = out[j - 1];
            --j;
        }
        out[j] = key;
    }

    *cnt = total;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* undefined, but safe fallback */

    static bool ready = false;
    static seq_t divisors[16];
    static size_t divisors_cnt = 0;

    if (!ready) {
        build_divisors(divisors, &divisors_cnt);
        ready = true;
    }

    if ((idx_t)divisors_cnt < n)
        return 0;          /* undefined for indices beyond 16 */

    return divisors[(idx_t)n - 1];
}
