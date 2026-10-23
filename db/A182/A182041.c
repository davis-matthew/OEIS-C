/*
 * A182041  Number of independent sets of nodes in C_5 X C_n (n >= 1).
 * https://oeis.org/A182041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=4*a(n-1)+27*a(n-2)+10*a(n-3)-30*a(n-4)-7*a(n-5)+8*a(n-6)-a(n-7);
   initial terms: a(0)=11, a(1)=1, a(2)=81, a(3)=391, a(4)=3561, a(5)=25531, a(6)=199821 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
# include <vector>
#endif

static bool initialized = false;
static idx_t last_computed = 0;

#ifdef __cplusplus
static std::vector<seq_t> buf;
#else
static seq_t *buf = NULL;
#endif

static void ensure_initialized(idx_t n)
{
    idx_t required = (n > 6) ? (n + 1) : 7;   /* at least size 7 for initial values */
    if (initialized) return;

#ifdef __cplusplus
    buf.resize(required);
#else
    buf = (seq_t *)malloc(required * sizeof(seq_t));
    if (!buf) exit(1);
#endif

    /* set the seven initial values */
    buf[0] = 11;
    buf[1] = 1;
    buf[2] = 81;
    buf[3] = 391;
    buf[4] = 3561;
    buf[5] = 25531;
    buf[6] = 199821;

    last_computed = 6;
    initialized = true;
}

static void grow_to(idx_t n)
{
    if (n <= last_computed) return;

#ifdef __cplusplus
    buf.resize(n + 1);
#else
    seq_t *tmp = (seq_t *)realloc(buf, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    buf = tmp;
#endif

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        buf[i] = 4 * buf[i - 1]
              + 27 * buf[i - 2]
              + 10 * buf[i - 3]
              - 30 * buf[i - 4]
              - 7  * buf[i - 5]
              + 8  * buf[i - 6]
              - buf[i - 7];
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    ensure_initialized(n);
    grow_to(n);
    return buf[n];
}
