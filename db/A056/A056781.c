/*
 * A056781  Prime powers such that the 4th power of the number of divisors is
 *          not smaller than the number itself.
 * https://oeis.org/A056781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = prime powers p^e such that (e+1)^4 >= p^e, sorted
 *            ascending
 * Range:     u8 n <= 19, u16 n <= 31, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: full, pass, 32 terms through n = 32 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: comparison of unsigned __int128 values */
static inline bool u128_le(unsigned __int128 a, unsigned __int128 b) {
    return a <= b;
}

/* Build the complete, sorted sequence once. */
static void build_sequence(unsigned __int128 *arr, int *cnt) {
    const int primes[] = {2, 3, 5, 7, 11, 13};
    const int num_primes = sizeof(primes) / sizeof(primes[0]);

    int idx = 0;
    for (int pi = 0; pi < num_primes; ++pi) {
        unsigned __int128 p = (unsigned __int128)primes[pi];
        int e = 1;
        unsigned __int128 power = p;          /* p^e */
        while (true) {
            unsigned __int128 d = (unsigned __int128)(e + 1);
            unsigned __int128 rhs = d * d * d * d;   /* (e+1)^4 */
            if (!u128_le(power, rhs))
                break;
            arr[idx++] = power;
            power *= p;
            ++e;
        }
    }

    /* Simple insertion sort for such a small array. */
    for (int i = 1; i < idx; ++i) {
        unsigned __int128 key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }

    *cnt = idx;
}

/* The types seq_t and idx_t are supplied by the build system. */

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static storage for the sequence. */
    static unsigned __int128 seq[64];
    static int seq_count = 0;
    static bool built = false;

    if (!built) {
        build_sequence(seq, &seq_count);
        built = true;
    }

    if (n == 0 || n > (idx_t)seq_count)
        return (seq_t)0;          /* out‑of‑range */

    return (seq_t)seq[(int)(n - 1)];
}
