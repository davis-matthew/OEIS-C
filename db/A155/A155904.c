/*
 * A155904  Number of ways to write 2n-1 as p+2^x+5*2^y with p an odd prime
 *          and x,y positive integers.
 * https://oeis.org/A155904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of representations of 2n-1 as p + 2^x + 5·2^y
 *            where p is an odd prime and x,y are positive integers
 * Range:     u8 n <= 64, u16 n <= 8982, u32 n <= 8982, u64 n <= 8982,
 *            u128 n <= 8982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_M 20000          /* maximum value of 2n-1 for n<=10000 */

static bool prime_tbl[MAX_M + 1];
static bool tbl_inited = false;

static void init_prime_tbl(void) {
    if (tbl_inited) return;
    tbl_inited = true;
    for (idx_t i = 0; i <= MAX_M; ++i) prime_tbl[i] = true;
    prime_tbl[0] = prime_tbl[1] = false;
    for (idx_t i = 2; i * i <= MAX_M; ++i) {
        if (!prime_tbl[i]) continue;
        for (idx_t j = i * i; j <= MAX_M; j += i) prime_tbl[j] = false;
    }
}

seq_t a(idx_t n) {
    init_prime_tbl();

    seq_t count = 0;
    seq_t M = (seq_t)(2 * n - 1);        /* 2n-1 */

    for (idx_t x = 1; ; ++x) {
        seq_t pow2x = (seq_t)1 << x;
        if (pow2x >= M) break;   /* 2^x must be strictly less than M */
        seq_t diff = M - pow2x;
        for (idx_t y = 1; ; ++y) {
            seq_t pow2y = (seq_t)1 << y;
            seq_t term5 = (seq_t)5 * pow2y;
            if (term5 > diff) break;
            seq_t val = diff - term5;
            if (val <= 1) continue;          /* p must be >1 */
            if ((val & 1) == 0) continue;    /* p must be odd */
            idx_t p = (idx_t)val;
            /* val fits into idx_t because M <= MAX_M < 2^16 */
            if (p <= MAX_M && prime_tbl[p]) ++count;
        }
    }

    return (seq_t)count;
}
