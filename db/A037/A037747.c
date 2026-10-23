/*
 * A037747  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7a(n-1) + a(n-4) – 7a(n-5); Initial values: a(1)=2,
 *            a(2)=17, a(3)=119, a(4)=834, a(5)=5840
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[10001];            /* stores a(1) … a(10000), index 0 unused */
static idx_t computed = 0;            /* highest index already computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n<1, safe return */

    /* initialise base values once */
    if (computed == 0) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)119;
        cache[4] = (seq_t)834;
        cache[5] = (seq_t)5840;
        computed = 5;
    }

    if (n <= computed) return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 7*a(i-1) + a(i-4) – 7*a(i-5) */
        seq_t term = cache[i - 1] * ((seq_t)7)
                   + cache[i - 4]
                   - cache[i - 5] * ((seq_t)7);
        cache[i] = term;
    }
    computed = n;
    return cache[n];
}
