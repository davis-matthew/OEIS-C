/*
 * A089089  Convoluted convolved Fibonacci numbers G_j^(2).
 * https://oeis.org/A089089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (c_{n-1} - b_{n-1}) / 2 where c_k = Σ_{i=0}^k g_i g_{k-i}
 *            with g_i = F_{i+1} and b_k = F_{k/2+1} if k is even, 0
 *            otherwise. The recurrence c_k =
 *            2c_{k-1}+c_{k-2}-2c_{k-3}-c_{k-4} (k>=4) allows linear‑time
 *            computation
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_arr = NULL;   /* fib_arr[k] = F_{k+1}  (Fib sequence) */
static idx_t fib_len = 0;
static seq_t *c_arr = NULL;     /* c_arr[k] = c_k as defined above */
static idx_t c_len = 0;

/* Ensure fib_arr has size at least sz (indices 0..sz-1) */
static void ensure_fib(idx_t sz)
{
    if (fib_len >= sz) return;
    /* grow: double size or reach sz, whichever larger */
    idx_t new_sz = fib_len ? fib_len * 2 : 2;
    if (new_sz < sz) new_sz = sz;
    fib_arr = (seq_t *)realloc(fib_arr, new_sz * sizeof(seq_t));
    if (!fib_arr) exit(1);
    /* initialize new elements */
    if (fib_len == 0) { /* first two values */
        fib_arr[0] = 1; /* F1 */
        fib_arr[1] = 1; /* F2 */
        fib_len = 2;
    }
    /* compute new Fibonacci values */
    for (idx_t i = fib_len; i < new_sz; ++i) {
        fib_arr[i] = fib_arr[i-1] + fib_arr[i-2];
    }
    fib_len = new_sz;
}

/* Ensure c_arr has size at least sz (indices 0..sz-1) */
static void ensure_c(idx_t sz)
{
    if (c_len >= sz) return;
    idx_t new_sz = c_len ? c_len * 2 : 4;
    if (new_sz < sz) new_sz = sz;
    c_arr = (seq_t *)realloc(c_arr, new_sz * sizeof(seq_t));
    if (!c_arr) exit(1);
    if (c_len == 0) {
        /* initialize first four values (convolution of Fibonacci) */
        c_arr[0] = 1;  /* c0 = 1*1 */
        c_arr[1] = 2;  /* c1 = 1*1 + 1*1 */
        c_arr[2] = 5;  /* c2 = 1*2 + 1*1 + 2*1 */
        c_arr[3] = 10; /* c3 = 1*3 + 1*2 + 2*1 + 3*1 */
        c_len = 4;
    }
    for (idx_t i = c_len; i < new_sz; ++i) {
        /* recurrence: c_i = 2c_{i-1} + c_{i-2} - 2c_{i-3} - c_{i-4} */
        seq_t tmp = c_arr[i-1] + c_arr[i-1]; /* 2c_{i-1} */
        tmp += c_arr[i-2];
        tmp -= c_arr[i-3] + c_arr[i-3];
        tmp -= c_arr[i-4];
        c_arr[i] = tmp;
    }
    c_len = new_sz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but safe guard */
    idx_t needed = n - 1;           /* index into c and b arrays */

    /* Prepare Fibonacci values up to floor(needed/2) */
    idx_t fib_needed = (needed >> 1) + 1;   /* ceil((needed+1)/2) */
    ensure_fib(fib_needed);

    /* Prepare c array up to needed */
    ensure_c(needed + 1);

    /* b[needed] = 0 if odd, else fib[needed/2] */
    seq_t b_val = (needed & 1) ? (seq_t)0 : fib_arr[needed >> 1];

    /* a(n) = (c[needed] - b_val) / 2 */
    seq_t res = (c_arr[needed] - b_val) / 2;
    return res;
}
