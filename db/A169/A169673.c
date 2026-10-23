/*
 * A169673  Lexicographically earliest de Bruijn sequence for n = 7 and k = 2.
 * https://oeis.org/A169673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* This function returns the nth term of the lexicographically earliest binary
   de Bruijn sequence of order 7 (period 128). a(n) is the nth bit of this
   sequence. */
#define N   7
#define K   2
#define LEN (1U << N)

static void _db(int t, int p, seq_t *seq, int *pos, int *a)
{
    if (t > N) {
        if (N % p == 0) {
            for (int i = 1; i <= p; ++i)
                seq[(*pos)++] = (seq_t)a[i];
        }
    } else {
        a[t] = a[t - p];
        _db(t + 1, p, seq, pos, a);
        for (int j = a[t - p] + 1; j < K; ++j) {
            a[t] = j;
            _db(t + 1, t, seq, pos, a);
        }
    }
}

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t period[LEN];
    if (!ready) {
        int a_arr[N + 1] = {0};   /* a[0] unused; a[1..N] form the word variables */
        int pos = 0;
        _db(1, 1, period, &pos, a_arr);
        ready = true;
    }
    return period[n % LEN];
}
