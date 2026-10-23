/*
 * A103715  Define a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=18, a(6)=22, a(7)=119,
 *          a(8)=285. Then a(n) = a(n-8) + 4*sqrt(420*a(n-4)^2 + 420*a(n-4) +
 *          1).
 * https://oeis.org/A103715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103715
 *   a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=18, a(6)=22, a(7)=119, a(8)=285,
 *   a(9)=1516
 *   a(n) = a(n-1) + 82*a(n-4) - 82*a(n-5) - a(n-8) + a(n-9)   (n ≥ 10)
 *
 * The function a(idx_t n) returns a(n) (1‑based).  A static table holds
 * previously computed values; the table grows only up to a maximum
 * index (see MAX_COMPUTE).  This implementation is valid C11 and C++17
 * and respects the constraints on allowed operations and headers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef MAX_COMPUTE
/* With n up to about 10000 the table size of 200000 is more than
   sufficient for all tests, yet still modest for memory consumption. */
#define MAX_COMPUTE 200000
#endif

/*  seq_t and idx_t are provided by the surrounding build system. */
static seq_t memo[MAX_COMPUTE + 1];   /* memo[1] … memo[MAX_COMPUTE]   */
static idx_t memo_computed = 0;       /* highest index for which memo[] is valid */

/* Initialise the fixed part of the sequence (a(1) … a(9)). */
static void init_base(void)
{
    if (memo_computed >= 9) return;   /* already initialised */

    memo[1] = (seq_t)0;          /* a(1) */
    memo[2] = (seq_t)0;          /* a(2) */
    memo[3] = (seq_t)1;          /* a(3) */
    memo[4] = (seq_t)3;          /* a(4) */
    memo[5] = (seq_t)18;         /* a(5) */
    memo[6] = (seq_t)22;         /* a(6) */
    memo[7] = (seq_t)119;        /* a(7) */
    memo[8] = (seq_t)285;        /* a(8) */
    memo[9] = (seq_t)1516;       /* a(9) */
    memo_computed = 9;
}

/* Compute the sequence up to and including index n.
   The table memo[] must be large enough: caller ensures n ≤ MAX_COMPUTE. */
static void compute_to(idx_t n)
{
    init_base();                       /* guarantee the base values are set */

    /* For n ≤ 9 we are already ready.  Otherwise iterate from the next
       already-computed position up to n. */
    for (idx_t i = memo_computed + 1; i <= n; ++i) {
        /* The recurrence is valid for i ≥ 10; for i ≤ 9 the initial
           values are already stored. */
        seq_t val = memo[i-1]                     /* a(i-1)          */
                  + (seq_t)82 * memo[i-4]         /* +82*a(i-4)      */
                  - (seq_t)82 * memo[i-5]         /* -82*a(i-5)      */
                  - memo[i-8]                     /* -a(i-8)         */
                  + memo[i-9];                    /* +a(i-9)         */
        memo[i] = val;
    }
    memo_computed = (memo_computed > n) ? memo_computed : n;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined for n < 1 but harmless */

    /* Ensure we have a table large enough.  For n larger than the
       compile‑time limit the function falls back to a simple on‑demand
       computation in a temporary ring buffer. */
    if (n <= MAX_COMPUTE) {
        if (n > memo_computed) compute_to(n);
        return memo[n];
    }

    /* n > MAX_COMPUTE: compute on the fly using a 9‑state buffer. */
    /* The recurrence only needs values from i-9 to i-1. */
    seq_t buf[9];
    idx_t i;

    /* Initialise buffer with the known first 9 terms. */
    init_base();
    for (i = 0; i < 9; ++i) buf[i] = memo[i+1];

    for (i = 10; i <= n; ++i) {
        seq_t next = buf[8]                                 /* a(i-1)  */
                   + (seq_t)82 * buf[4]                     /* +82*a(i-4) */
                   - (seq_t)82 * buf[3]                     /* -82*a(i-5) */
                   - buf[0]                                 /* -a(i-8)  */
                   + buf[8-1];                              /* +a(i-9)  */
        /* rotate the buffer: drop oldest, shift left, insert new. */
        for (idx_t k = 0; k < 8; ++k) buf[k] = buf[k+1];
        buf[8] = next;
    }

    return buf[8];
}
