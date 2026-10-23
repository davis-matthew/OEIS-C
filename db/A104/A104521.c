/*
 * A104521  Fixed point of the morphism 0->{1}, 1->{1,0,1}.
 * https://oeis.org/A104521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+1)/sqrt(2)) - round(n/sqrt(2))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate the fixed‑point word of the morphism
   0 → 1, 1 → 101 until it contains the n‑th element. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *buf      = NULL;          /* current word          */
static idx_t  cur_len  = 0;             /* length of buf          */
static bool   init_done = false;        /* initialisation flag    */
static const idx_t MAX_CAP = 20000;     /* safe upper bound for n ≤ 10000 */

/* Initialise the buffer with the first word "1".  */
static void init(void)
{
    if (init_done) return;
    buf = (seq_t *)malloc(MAX_CAP * sizeof(seq_t));
    if (!buf) abort();                  /* allocation failed */
    buf[0] = 1;
    cur_len = 1;
    init_done = true;
}

/* Grow the word until it contains indices 0 … n.  */
static void grow_until(idx_t n)
{
    init();
    while (cur_len <= n)
    {
        /* Count how many 1’s are in the current word. */
        idx_t ones = 0;
        for (idx_t i = 0; i < cur_len; ++i)
            if (buf[i] == 1)
                ++ones;

        /* Compute length of the next word. */
        idx_t new_len = ones * 3 + (cur_len - ones);
        if (new_len > MAX_CAP)
            abort();                     /* should not happen for n ≤ 10000 */

        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!next) abort();

        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i)
        {
            if (buf[i] == 1)
            {
                next[pos++] = 1;
                next[pos++] = 0;
                next[pos++] = 1;
            }
            else /* buf[i] == 0 */
                next[pos++] = 1;
        }

        free(buf);
        buf = next;
        cur_len = new_len;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    grow_until(n);
    return buf[n];
}
