/*
 * A093792  Hook products of all partitions of 13.
 * https://oeis.org/A093792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 13!/A003877(102-n), where A003877 lists the degrees
 *            f^lambda of the irreducible representations of S_13 in
 *            increasing order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Validated: full, pass, 101 terms through n = 101 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t A093792_syt_count(int *part, int plen) {
    if (plen == 0) return 1;
    seq_t total = 0;
    for (int i = 0; i < plen; i++) {
        if (i == plen - 1 || part[i] > part[i + 1]) {
            part[i]--;
            int newlen = (part[i] == 0) ? plen - 1 : plen;
            total += A093792_syt_count(part, newlen);
            part[i]++;
        }
    }
    return total;
}

static seq_t A093792_deg[101];
static int A093792_size = 0;
static int A093792_ready = 0;

static void A093792_gen(int *part, int plen, int remaining, int max_part) {
    if (remaining == 0) {
        if (A093792_size < 101) {
            A093792_deg[A093792_size++] = A093792_syt_count(part, plen);
        }
        return;
    }
    int limit = (remaining < max_part) ? remaining : max_part;
    for (int p = limit; p >= 1; p--) {
        part[plen] = p;
        A093792_gen(part, plen + 1, remaining - p, p);
    }
}

static void A093792_init(void) {
    if (A093792_ready) return;
    int part[14];
    A093792_size = 0;
    A093792_gen(part, 0, 13, 13);
    for (int i = 1; i < A093792_size; i++) {
        seq_t key = A093792_deg[i];
        int j = i - 1;
        while (j >= 0 && A093792_deg[j] > key) {
            A093792_deg[j + 1] = A093792_deg[j];
            j--;
        }
        A093792_deg[j + 1] = key;
    }
    A093792_ready = 1;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 101) return 0;
    A093792_init();
    seq_t f = A093792_deg[101 - (int)n];
    seq_t fact = 1;
    for (int i = 2; i <= 13; i++) fact *= (seq_t)i;
    return fact / f;
}
