/*
 * A073964  Number of strings of length n over Z_5 with trace 0 and subtrace
 *          1.
 * https://oeis.org/A073964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let a(n; t, s) be the number of strings of length n
   over Z_5 with trace t and subtrace s (mod 5). Then

   a(n; t, s) = a(n‑1; t, s)
               + a(n‑1; (t+4) % 5, (s+4*t+1) % 5)
               + a(n‑1; (t+3) % 5, (s+3*t+4) % 5)
               + a(n‑1; (t+2) % 5, (s+2*t+4) % 5)
               + a(n‑1; (t+1) % 5, (s+t+1) % 5),

   with the boundary condition a(0; 0,0)=1 and a(0; t,s)=0 for (t,s)≠(0,0).
   The required sequence is a(n)=a(n;0,1).  This function implements the
   dynamic‑programming evaluation of that recurrence.  It memoises
   intermediate values so calls in any order are fast. */

#include <stdlib.h>

static idx_t cur_len = 0;            /* longest n already built */
static seq_t dp_cur[25];            /* DP state for current length */
static seq_t dp_next[25];           /* DP state for next length */
static seq_t *a_cache = NULL;       /* cache of a(n) values */
static idx_t cache_size = 0;        /* allocated size of a_cache */

/* initialise dp_cur for length 0 (empty string) */
static void init_dp(void)
{
    static int once = 0;
    if (once) return;
    once = 1;
    for (int i = 0; i < 25; ++i) dp_cur[i] = (seq_t)0;
    dp_cur[0] = (seq_t)1;          /* trace 0, sub‑trace 0 */
    cur_len = 0;
}

/* extend a_cache to hold values up to at least n */
static void extend_cache(idx_t n)
{
    if (cache_size >= n + 1) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (new_cache == NULL) abort();   /* out of memory */
    /* copy existing elements */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = a_cache[i];
    /* initialise new entries to 0 */
    for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
    free(a_cache);
    a_cache = new_cache;
    cache_size = new_size;
}

/* compute a(n) ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but harmless */

    init_dp();
    extend_cache(n);

    /* if we already computed up to n, just return the stored value */
    if (cur_len >= n) return a_cache[n];

    /* pre‑compute a² mod 5 for a = 0..4 */
    static const int aa[5] = {0, 1, 4, 4, 1};

    /* iterate lengths from cur_len+1 up to n */
    for (idx_t len = cur_len + 1; len <= n; ++len)
    {
        /* zero dp_next */
        for (int i = 0; i < 25; ++i) dp_next[i] = (seq_t)0;

        /* transition from dp_cur to dp_next */
        for (int t = 0; t < 5; ++t)
        {
            for (int s = 0; s < 5; ++s)
            {
                seq_t val = dp_cur[t * 5 + s];
                if (val == (seq_t)0) continue;
                for (int a = 0; a < 5; ++a)
                {
                    int t2 = (t + a) % 5;
                    int s2 = (s + a * t + aa[a]) % 5;
                    int idx2 = t2 * 5 + s2;
                    dp_next[idx2] += val;
                }
            }
        }

        /* copy dp_next into dp_cur for next iteration */
        for (int i = 0; i < 25; ++i) dp_cur[i] = dp_next[i];

        /* store current a(len) = dp_cur[0][1] */
        a_cache[len] = dp_cur[0 * 5 + 1];
    }

    cur_len = n;
    return a_cache[n];
}
