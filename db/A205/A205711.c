/*
 * A205711  Positions of multiples of 9 in A204890 (differences of primes).
 * https://oeis.org/A205711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k = (j-1)(j-2)/2 + i (1<=i<j) with prime(j) ==
 *            prime(i) (mod 9), i.e. 9 | prime(j)-prime(i); found by
 *            incremental search over rows of A204890
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static uint32_t *pr = NULL;
    static size_t pcnt = 0, pcap = 0;
    static uint64_t *terms = NULL;
    static size_t tcnt = 0, tcap = 0;

    if (n == 0) return (seq_t)0;

    while (tcnt < (size_t)n) {
        /* next prime */
        uint32_t p;
        if (pcnt == 0) p = 2;
        else if (pcnt == 1) p = 3;
        else {
            p = pr[pcnt - 1] + 2;
            for (;;) {
                bool ok = true;
                for (size_t k = 1; k < pcnt; ++k) {
                    uint64_t q = pr[k];
                    if (q * q > p) break;
                    if (p % pr[k] == 0) { ok = false; break; }
                }
                if (ok) break;
                p += 2;
            }
        }
        if (pcnt == pcap) {
            pcap = pcap ? pcap * 2 : 64;
            pr = (uint32_t *)realloc(pr, pcap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[pcnt++] = p;

        /* row j = pcnt, pairs i = 1..j-1 */
        uint64_t j = pcnt;
        uint64_t base = (j - 1) * (j - 2) / 2;
        uint32_t r = p % 9;
        for (uint64_t i = 1; i < j; ++i) {
            if (pr[i - 1] % 9 == r) {
                if (tcnt == tcap) {
                    tcap = tcap ? tcap * 2 : 64;
                    terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
                    if (!terms) abort();
                }
                terms[tcnt++] = base + i;
            }
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
