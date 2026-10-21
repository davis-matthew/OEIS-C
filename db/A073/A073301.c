/*
 * A073301  a(n) is the n-th new record value in A073300.
 * https://oeis.org/A073301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 73, u64 n <= 149,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073301:
 * a(n) = n‑th new record value of A073300.
 * Recurrence for n ≥ 7:
 *   a(3n)   = a(3n-2) + a(3n-4)
 *   a(3n+1) = a(3n-1) + a(3n-2)
 *   a(3n+2) = a(3n+1) + a(3n-2)
 * Initial values (offset 1):
 *   a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=8,
 *   a(7)=12, a(8)=17, a(9)=19, a(10)=29, a(11)=41,
 *   a(12)=46, a(13)=70, a(14)=99, a(15)=111,
 *   a(16)=169, a(17)=239, a(18)=268, a(19)=408,
 *   a(20)=577
 */

static seq_t cache[50001] = {0};   /* cache[1] .. cache[50000] */
static idx_t cache_max   = 0;      /* highest index currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0 */

    /* Initialise the base values once */
    if (cache_max == 0) {
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)2;
        cache[3]  = (seq_t)3;
        cache[4]  = (seq_t)5;
        cache[5]  = (seq_t)7;
        cache[6]  = (seq_t)8;
        cache[7]  = (seq_t)12;
        cache[8]  = (seq_t)17;
        cache[9]  = (seq_t)19;
        cache[10] = (seq_t)29;
        cache[11] = (seq_t)41;
        cache[12] = (seq_t)46;
        cache[13] = (seq_t)70;
        cache[14] = (seq_t)99;
        cache[15] = (seq_t)111;
        cache[16] = (seq_t)169;
        cache[17] = (seq_t)239;
        cache[18] = (seq_t)268;
        cache[19] = (seq_t)408;
        cache[20] = (seq_t)577;
        cache_max = 20;
    }

    if (n <= cache_max) return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i % 3 == 0) {
            /* a(3k)   = a(3k-2) + a(3k-4) */
            cache[i] = cache[i-2] + cache[i-4];
        } else if (i % 3 == 1) {
            /* a(3k+1) = a(3k-1) + a(3k-2) */
            cache[i] = cache[i-2] + cache[i-3];
        } else { /* i % 3 == 2 */
            /* a(3k+2) = a(3k+1) + a(3k-2) */
            cache[i] = cache[i-1] + cache[i-4];
        }
    }

    cache_max = n;
    return cache[n];
}
