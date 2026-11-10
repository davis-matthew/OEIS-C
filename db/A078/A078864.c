/*
 * A078864  Smallest primes from A001359, each belonging to those different
 *          residue class of mod 210 which are listed in A078859. Arranged
 *          according to possible least positive residues mod 210.
 * https://oeis.org/A078864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n-th smallest of { least twin prime p (A001359) in each
 *            residue class mod 210 that contains a lesser twin prime };
 *            computed by search (trial-division scan of twin primes), sorted
 *            ascending
 * Range:     u8 n <= 15, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a078864_isprime(uint64_t x) {
    uint64_t d;
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    static uint64_t vals[32];
    static int cnt = 0;
    static bool init = false;
    if (!init) {
        uint64_t first[210];
        uint64_t p;
        int i, j;
        for (i = 0; i < 210; i++) first[i] = 0;
        for (p = 3; p < 5000; p += 2) {
            if (a078864_isprime(p) && a078864_isprime(p + 2)) {
                int r = (int)(p % 210);
                if (first[r] == 0) first[r] = p;
            }
        }
        for (i = 0; i < 210; i++) {
            if (first[i] != 0 && cnt < 32) vals[cnt++] = first[i];
        }
        for (i = 1; i < cnt; i++) {
            uint64_t v = vals[i];
            j = i - 1;
            while (j >= 0 && vals[j] > v) {
                vals[j + 1] = vals[j];
                j--;
            }
            vals[j + 1] = v;
        }
        init = true;
    }
    if (n == 0 || n > (idx_t)cnt) return 0;
    return (seq_t)vals[(int)(n - 1)];
}
