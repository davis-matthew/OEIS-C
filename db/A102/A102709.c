/*
 * A102709  Let a(n,m) = card{f^(n) : f is a mapping from a set of m elements
 *          into itself}, where f^(l)(x) = f^(l-1)(f(x)),l>0, f^(0)(x) = x;
 *          sequence gives a(n,5).
 * https://oeis.org/A102709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = card{f^n : f is a mapping from a set of 5 elements into
 *            itself}
 * Range:     u8 n <= 0, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*----- constants -----------------------------------------------------*/
#define M      5
#define TOTAL  3125   /* 5^5 */

/*----- global tables --------------------------------------------------*/
static seq_t a_tbl[64];               /* a(0) … a(63) */
static unsigned char f_map[TOTAL][M]; /* encodes all functions */
static bool f_ready = false;

/*----- helper: precompute tables -------------------------------------*/
static void init_tables(void)
{
    if (f_ready) return;
    f_ready = true;

    /* encode all functions as base‑5 numbers */
    for (unsigned int id = 0; id < TOTAL; ++id) {
        unsigned int tmp = id;
        for (int i = 0; i < M; ++i) {
            f_map[id][i] = tmp % M;
            tmp /= M;
        }
    }

    /* compute a_tbl[0 … 63] */
    for (unsigned int n = 0; n < 64; ++n) {
        bool seen[TOTAL] = { 0 };

        for (unsigned int f = 0; f < TOTAL; ++f) {
            unsigned char cur[M];
            unsigned char g[M];

            /* start with f^1 = f itself */
            for (int i = 0; i < M; ++i) cur[i] = f_map[f][i];

            if (n == 0) {
                /* f^0 is the identity */
                for (int i = 0; i < M; ++i) g[i] = (unsigned char)i;
            } else {
                /* iterate n-1 times to obtain f^n */
                for (unsigned int step = 1; step < n; ++step) {
                    unsigned char nxt[M];
                    for (int i = 0; i < M; ++i)
                        nxt[i] = f_map[f][cur[i]];
                    for (int i = 0; i < M; ++i) cur[i] = nxt[i];
                }
                for (int i = 0; i < M; ++i) g[i] = cur[i];
            }

            /* encode g as an integer 0 … TOTAL‑1 */
            unsigned int idg = 0;
            unsigned int pow = 1;
            for (int i = 0; i < M; ++i) {
                idg += g[i] * pow;
                pow  *= M;
            }
            seen[idg] = true;
        }

        /* count distinct iterates */
        unsigned int cnt = 0;
        for (unsigned int i = 0; i < TOTAL; ++i)
            if (seen[i]) ++cnt;
        a_tbl[n] = (seq_t)cnt;
    }
}

/*----- public sequence function -------------------------------------*/
seq_t a(idx_t n)
{
    /* delay initialization until first use */
    static bool inited = false;
    if (!inited) { inited = true; init_tables(); }

    if (n < 4ULL)
        return a_tbl[(unsigned int)n];

    /* a(n) is periodic with period 60 for n ≥ 4 */
    idx_t mod = (n - 4ULL) % 60ULL;
    return a_tbl[(unsigned int)(4 + mod)];
}
