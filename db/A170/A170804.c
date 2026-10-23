/*
 * A170804  Partial sums of (A006899, prefixed by a 1).
 * https://oeis.org/A170804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{k=1}^{n-1} A006899(k) where A006899(k) is the
 *            k‑th smallest number that is a power of 2 or a power of 3
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *a_arr = NULL;      /* array of a(n) values, 1‑based */
static size_t arr_cap = 0;       /* allocated capacity of a_arr */
static size_t arr_len = 0;       /* highest index computed so far */

static seq_t nextPow2 = (seq_t)2; /* next power of 2 to be inserted into A006899 */
static seq_t nextPow3 = (seq_t)3; /* next power of 3 to be inserted into A006899 */
static seq_t last_f   = (seq_t)1; /* the last computed value of A006899 */

static void ensure_capacity(size_t needed)
{
    if (needed+1 <= arr_cap) return;            /* we always keep a[0] unused */
    size_t new_cap = (arr_cap == 0) ? 128 : arr_cap * 2;
    while (new_cap <= needed) new_cap *= 2;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    arr_cap = new_cap;
}

/* lazily initialise state on the first call */
static void init_state(void)
{
    if (a_arr != NULL) return;                   /* already initialised */

    a_arr  = (seq_t *)malloc(128 * sizeof(seq_t));
    arr_cap = 128;
    a_arr[1] = (seq_t)1;                         /* a(1) = 1               */
    arr_len  = 1;                                /* we have computed a(1)  */

    /* prepare the first powers of 2 and 3 to be inserted into A006899 */
    nextPow2 = (seq_t)2;                         /* 2^1 */
    nextPow3 = (seq_t)3;                         /* 3^1 */
    last_f   = (seq_t)1;                         /* last A006899 value used (f(1)) */
}

/* public function requested by the problem statement */
seq_t a(idx_t n)
{
    init_state();

    if (n < 1) return (seq_t)0;          /* undefined for n<1, but return 0 */

    size_t target = (size_t)n;            /* convert idx_t to size_t safely */

    if (target > arr_len) {
        ensure_capacity(target);
        for (size_t i = arr_len + 1; i <= target; ++i) {
            /* a[i] = a[i-1] + f(i-1) where f(i-1) = last_f */
            a_arr[i] = a_arr[i-1] + last_f;

            /* compute f(i) for the next step: merge two sorted lists of
               powers of 2 and powers of 3 */
            seq_t next_f;
            if (nextPow2 <= nextPow3) {
                next_f = nextPow2;
                nextPow2 = nextPow2 * (seq_t)2;   /* advance 2‑power */
            } else {
                next_f = nextPow3;
                nextPow3 = nextPow3 * (seq_t)3;   /* advance 3‑power */
            }
            last_f = next_f;
        }
        arr_len = target;
    }

    return a_arr[target];
}
