/*
 * A106437  Fixed point of the morphism 1->4, 2->1222, 3->2, 4->3444, applied
 *          4*k times to 4.
 * https://oeis.org/A106437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106437
   Fixed point of the morphism 1→4, 2→1222, 3→2, 4→3444,
   applied 4·k times to 4.  a(0)=4. */

#include <stdint.h>

/* maximum depth needed for n up to the limits of the tests */
#define MAX_T 32
#define MAX_M_LEN 256

/* four‑fold expansion of each symbol (M[s]) */
static uint8_t M[5][MAX_M_LEN];
static uint8_t M_len[5];

/* len[t][s] = length of the word obtained by applying M t times to symbol s */
static idx_t len[MAX_T][5];
static int len_tmax = -1;

/* initialise the table M */
static void init_M(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    uint8_t cur[MAX_M_LEN];
    uint8_t nxt[MAX_M_LEN];

    for (int s = 1; s <= 4; ++s)
    {
        int cur_len = 1;
        cur[0] = (uint8_t)s;

        for (int it = 0; it < 4; ++it) /* apply S four times */
        {
            int next_len = 0;
            for (int i = 0; i < cur_len; ++i)
            {
                switch (cur[i])
                {
                case 1:
                    nxt[next_len++] = 4;
                    break;
                case 2:
                    nxt[next_len++] = 1;
                    nxt[next_len++] = 2;
                    nxt[next_len++] = 2;
                    nxt[next_len++] = 2;
                    break;
                case 3:
                    nxt[next_len++] = 2;
                    break;
                case 4:
                    nxt[next_len++] = 3;
                    nxt[next_len++] = 4;
                    nxt[next_len++] = 4;
                    nxt[next_len++] = 4;
                    break;
                }
            }
            for (int i = 0; i < next_len; ++i) cur[i] = nxt[i];
            cur_len = next_len;
        }
        M_len[s] = cur_len;
        for (int i = 0; i < cur_len; ++i) M[s][i] = cur[i];
    }
}

/* ensure that len[] is computed up to length ≥ n for symbol 4 */
static void ensure_len(idx_t n)
{
    if (len_tmax == -1)
    {
        for (int s = 1; s <= 4; ++s) len[0][s] = 1;
        len_tmax = 0;
    }
    while (len[len_tmax][4] <= n && len_tmax + 1 < MAX_T)
    {
        int t = len_tmax + 1;
        for (int s = 1; s <= 4; ++s)
        {
            idx_t sum = 0;
            for (int i = 0; i < M_len[s]; ++i)
                sum += len[t - 1][M[s][i]];
            len[t][s] = sum;
        }
        len_tmax = t;
    }
}

seq_t a(idx_t n)
{
    init_M();
    ensure_len(n);

    int symbol = 4;       /* start from the symbol 4 */
    int t = len_tmax;     /* number of remaining M applications */
    idx_t pos = n;        /* position within the current word */

    while (t > 0)
    {
        for (int i = 0; i < M_len[symbol]; ++i)
        {
            uint8_t child = M[symbol][i];
            idx_t child_len = len[t - 1][child];
            if (pos < child_len)
            {
                symbol = child;
                break;
            }
            pos -= child_len;
        }
        t--;
    }

    return (seq_t)symbol;
}
