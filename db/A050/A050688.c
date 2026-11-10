/*
 * A050688  Numbers k such that A000422(k) is nonsquarefree.
 * https://oeis.org/A050688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard
 * Formula:   a(n) = n-th k with A000422(k) divisible by p^2 for some prime p
 *            <= 200000 (A000422(k) = concatenation k..1); search by modular
 *            residues
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A050688_KMAX 400
#define A050688_PMAX 200000

static int A050688_ready = 0;
static int A050688_terms[A050688_KMAX + 1];
static int A050688_count = 0;

static void A050688_init(void) {
    static unsigned char flag[A050688_KMAX + 1];
    static unsigned char comp[A050688_PMAX + 1];
    for (int k = 0; k <= A050688_KMAX; k++) flag[k] = 0;
    for (uint64_t i = 2; i <= A050688_PMAX; i++) {
        if (comp[i]) continue;
        for (uint64_t j = i * i; j <= A050688_PMAX; j += i) comp[j] = 1;
        uint64_t m = i * i;
        uint64_t r = 1 % m;
        uint64_t pw = 10 % m;
        for (int k = 2; k <= A050688_KMAX; k++) {
            r = ((uint64_t)k * pw + r) % m;
            int d = (k < 10) ? 1 : (k < 100) ? 2 : (k < 1000) ? 3 : 4;
            for (int t = 0; t < d; t++) pw = (pw * 10) % m;
            if (r == 0) flag[k] = 1;
        }
        if (m == 1) flag[1] = 1;
    }
    A050688_count = 0;
    for (int k = 1; k <= A050688_KMAX; k++)
        if (flag[k]) A050688_terms[A050688_count++] = k;
    A050688_ready = 1;
}

seq_t a(idx_t n) {
    if (!A050688_ready) A050688_init();
    if (n < 1 || n > (idx_t)A050688_count) return (seq_t)0;
    return (seq_t)A050688_terms[n - 1];
}
