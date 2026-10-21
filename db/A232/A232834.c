/*
 * A232834  Number of (n+1) X (2+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          diagonally or antidiagonally.
 * https://oeis.org/A232834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+18*a(n-2)+772*a(n-3)+844*a(n-4)+9204*a(n-5)+
 *            198402*a(n-9)+173348*a(n-10)+1920*a(n-12)+15360*a(n-13) //
 *            positive terms -
 *            (55335*a(n-6)+127538*a(n-7)+311040*a(n-8)+196912*a(n-11)) //
 *            negative terms Initial values: a(1)=16, a(2)=312, a(3)=3720,
 *            a(4)=44832, a(5)=500516, a(6)=5809256,...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memo_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* Undefined for n<1, but returns 0. */
    /* Ensure memo contains at least first 13 terms. */
    if (memo_size == 0) {
        memo = (seq_t *)malloc(13 * sizeof(seq_t));
        if (!memo) return 0;
        memo[0] = (seq_t)16;
        memo[1] = (seq_t)312;
        memo[2] = (seq_t)3720;
        memo[3] = (seq_t)44832;
        memo[4] = (seq_t)500516;
        memo[5] = (seq_t)5809256;
        memo[6] = (seq_t)71242892;
        memo[7] = (seq_t)828231244;
        memo[8] = (seq_t)9665239984;
        memo[9] = (seq_t)115029333056;
        memo[10] = (seq_t)1352576937428;
        memo[11] = (seq_t)15898702233248;
        memo[12] = (seq_t)187695013262932;
        memo_size = 13;
    }

    /* Grow memo if necessary. */
    idx_t target = n;
    if (target > (idx_t)memo_size) {
        size_t new_size = (size_t)target;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        memo = tmp;
        /* Compute terms from memo_size up to target-1. */
        for (size_t i = memo_size; i < new_size; ++i) {
            /* i corresponds to a(i+1). We need a(i) .. a(i-12). */
            seq_t pos = 0, neg = 0;
            pos += memo[i-1] * (seq_t)4;
            pos += memo[i-2] * (seq_t)18;
            pos += memo[i-3] * (seq_t)772;
            pos += memo[i-4] * (seq_t)844;
            pos += memo[i-5] * (seq_t)9204;
            pos += memo[i-9] * (seq_t)198402;
            pos += memo[i-10] * (seq_t)173348;
            pos += memo[i-12] * (seq_t)1920;
            pos += memo[i-13] * (seq_t)15360;
            neg += memo[i-6] * (seq_t)55335;
            neg += memo[i-7] * (seq_t)127538;
            neg += memo[i-8] * (seq_t)311040;
            neg += memo[i-11] * (seq_t)196912;
            memo[i] = pos - neg;
        }
        memo_size = new_size;
    }

    return memo[(size_t)n - 1];
}
