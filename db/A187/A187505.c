/*
 * A187505  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-1,0,1,2}, n=3*r+p_i, and define a(-1)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,3,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^3-2*x) with x=2*cos(Pi/9).
 * https://oeis.org/A187505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 3*a(n-6) - a(n-9) - a(n-12), for n >= 12 with
 *            initial values a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=1,
 *            a(6)=2, a(7)=3, a(8)=3, a(9)=6, a(10)=8, a(11)=9
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int a_initialized = 0;
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;
static idx_t a_compute_upto = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_size)
        return;
    /* grow cache to at least n+16 and round up to a power of two for efficiency */
    idx_t new_size = a_cache_size ? a_cache_size << 1 : 16;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_size);
    if (!new_cache)
        abort(); /* allocation failure */
    a_cache = new_cache;
    a_cache_size = new_size;
}

static void init_a(void)
{
    if (a_initialized)
        return;
    a_cache = NULL;
    a_cache_size = 0;
    a_compute_upto = 12; /* we precompute initial 12 values (0..11) */
    ensure_a_cache(a_compute_upto);
    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)0;
    a_cache[3] = (seq_t)1;
    a_cache[4] = (seq_t)1;
    a_cache[5] = (seq_t)1;
    a_cache[6] = (seq_t)2;
    a_cache[7] = (seq_t)3;
    a_cache[8] = (seq_t)3;
    a_cache[9] = (seq_t)6;
    a_cache[10] = (seq_t)8;
    a_cache[11] = (seq_t)9;
    a_initialized = 1;
}

seq_t a(idx_t n)
{
    init_a();
    if (n < a_compute_upto) /* previously computed */
        return a_cache[n];

    ensure_a_cache(n);
    /* compute from a_compute_upto to n */
    for (idx_t i = a_compute_upto; i <= n; ++i)
    {
        if (i < 12) /* already initialized above, but keep here for safety */
        {
            /* values already set by init_a */
        }
        else
        {
            /* recurrence: a(i) = 2*a(i-3) + 3*a(i-6) - a(i-9) - a(i-12) */
            seq_t t1 = a_cache[i - 3];
            seq_t t2 = a_cache[i - 6];
            seq_t t3 = a_cache[i - 9];
            seq_t t4 = a_cache[i -12];
            a_cache[i] = 2 * t1 + 3 * t2 - t3 - t4;
        }
    }
    a_compute_upto = n + 1; /* we now have up to n inclusive */
    return a_cache[n];
}
