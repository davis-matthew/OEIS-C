/*
 * A230448  T(n, k) = T(n-1, k-1) + T(n-1, k) with T(n, 0) = 1 and T(n, n) =
 *          A226205(n+1), n >= 0 and 0 <= k <= n.
 * https://oeis.org/A230448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A230448 (triangle T(n,k) flattened) */
/* Recurrence: T(n,k) = T(n-1,k-1) + T(n-1,k) with T(n,0)=1 and
   T(n,n) = F(n+2) * F(n-1),  where F are Fibonacci numbers.
   The flattened sequence a(n) lists the entries row‑wise: 
   row 0, row 1, row 2, …                     */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* auxiliary static storage -------------------------------------------------- */

static seq_t *fib      = NULL;   /* Fibonacci array, fib[0]=0, fib[1]=1        */
static size_t fib_len  = 0;      /* number of Fibonacci numbers computed      */

static seq_t *prev_row = NULL;   /* previous triangle row                     */
static size_t prev_len = 0;       /* length of previous row (row+1)            */

static seq_t *cur_row  = NULL;   /* buffer for the current row                */
static size_t cur_len  = 0;       /* allocated length of cur_row               */

static seq_t *flat     = NULL;   /* flattened sequence up to current_row      */
static size_t flat_len = 0;       /* number of elements in flat               */

static size_t cur_row_index = (size_t)-1;   /* last row computed, -1 initially */

/* -------------------------------------------------------------------------- */
/*  Ensure that fib array contains Fibonacci numbers up to index 'need'.      */
/*  F[0] = 0, F[1] = 1, F[n] = F[n-1] + F[n-2] for n >= 2.                */
static void
ensure_fibs(size_t need)
{
    if (need < fib_len) return;

    size_t new_cap = fib_len ? fib_len << 1 : 32;
    while (new_cap <= need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);

    if (fib_len == 0) {
        tmp[0] = 0;
        tmp[1] = 1;
        fib_len = 2;
    }

    for (size_t i = fib_len; i <= need; ++i)
        tmp[i] = tmp[i - 1] + tmp[i - 2];

    fib_len = need + 1;
    fib     = tmp;
}

/* -------------------------------------------------------------------------- */
/*  Compute the next triangle row (index cur_row_index+1) and append it to    */
/*  the flattened sequence.  Afterwards cur_row_index is increased.          */
static void
append_next_row(void)
{
    size_t next = cur_row_index + 1;          /* row index to compute */
    size_t len  = next + 1;                  /* number of elements in this row */

    /* resize cur_row buffer if necessary */
    if (cur_len < len) {
        seq_t *tmp = (seq_t *)realloc(cur_row, len * sizeof(seq_t));
        if (!tmp) exit(1);
        cur_row = tmp;
        cur_len = len;
    }

    /* first element always 1 */
    cur_row[0] = 1;

    /* interior elements via Pascal recurrence */
    if (next >= 1) {
        for (size_t k = 1; k < next; ++k)
            cur_row[k] = prev_row[k - 1] + prev_row[k];
    }

    /* last element using Fibonacci product */
    if (next >= 1) {
        ensure_fibs(next + 2);   /* need F(next+2) and F(next-1)          */
        cur_row[next] = fib[next + 2] * fib[next - 1];
    }

    /* append cur_row to flattened array */
    size_t new_flat_len = flat_len + len;
    seq_t *tmp = (seq_t *)realloc(flat, new_flat_len * sizeof(seq_t));
    if (!tmp) exit(1);
    flat = tmp;

    for (size_t i = 0; i < len; ++i)
        flat[flat_len + i] = cur_row[i];

    flat_len = new_flat_len;

    /* prepare for next iteration: prev_row now points to the row we
       have just produced.  Swap pointers to avoid copying the row.  */
    seq_t *swap = prev_row;
    prev_row   = cur_row;
    cur_row    = swap;
    prev_len   = len;
    cur_row_index = next;
}

/* -------------------------------------------------------------------------- */
/*  Public function required by the problem statement.                       */
seq_t a(idx_t n)
{
    /* If the requested index is beyond what we have computed, compute
       additional rows until flat_len > n. */
    while (flat_len <= (size_t)n) {
        /* First call: initialise the very first row (row 0).          */
        if (cur_row_index == (size_t)-1) {
            append_next_row();  /* will create row 0 */
        } else {
            append_next_row();
        }
    }

    /* Now flat[n] is valid, because flat_len > n. */
    return flat[(size_t)n];
}
