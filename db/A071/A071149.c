/*
 * A071149  Numbers n such that the sum of the first n odd primes (A071148) is
 *          prime; analogous to A013916.
 * https://oeis.org/A071149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that the sum of the first n odd
 *            primes is prime (implemented by incremental prime generation and
 *            primality testing)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 912 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Static state to allow non‑increasing calls */
    static seq_t *qual = NULL;
    static idx_t qlen = 0;
    static size_t qcap = 0;
    static uint64_t cand = 3;          /* current odd candidate to test for primality */
    static idx_t primecnt = 0;         /* count of odd primes seen so far (n) */
    static seq_t sum = 0;              /* running sum of odd primes */

    while (qlen < n) {
        /* Find the next odd prime */
        while (!is_prime_seq((seq_t)cand))
            cand += 2;

        /* Update state with this prime */
        primecnt++;
        sum += (seq_t)cand;

        /* If the sum is prime, record the current n */
        if (is_prime_big(sum)) {
            if (qlen == qcap) {
                size_t newcap = qcap ? qcap * 2 : 16;
                seq_t *newp = mem_new(seq_t, newcap);
                if (!newp) exit(1);
                if (qual) {
                    mem_copy(newp, qual, qlen * sizeof(seq_t));
                    mem_free(qual);
                }
                qual = newp;
                qcap = newcap;
            }
            qual[qlen++] = (seq_t)primecnt;
        }

        /* Next odd candidate */
        cand += 2;
    }

    return qual[n - 1];
}
