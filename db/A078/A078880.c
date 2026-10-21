/*
 * A078880  The sequence starting with 2 that equals its own run length
 *          sequence.
 * https://oeis.org/A078880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of A078880 – sequence starting at 2 with the property that
 *  the sequence equals its own run‑length sequence.  The algorithm generates
 *  the Kolakoski sequence starting with 2; the generated numbers themselves
 *  are the required sequence values.  The method keeps a running state
 *  so that calls to a(n) in arbitrary order are answered in O(1) (after the
 *  initial generation) and takes O(n) time for the smallest missing index.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static state shared by all calls to a()                            */
/* ------------------------------------------------------------------ */
static seq_t *seq_arr = NULL;   /* array of generated values */
static size_t  seq_cap = 0;     /* capacity of seq_arr            */
static size_t  seq_len = 0;     /* number of elements stored      */

/* Generation state: index into seq_arr used for the current run length */
static idx_t   gen_pos = 0;
static seq_t   gen_curr = 2;    /* value currently being repeated (1 or 2) */
static seq_t   gen_run  = 0;    /* remaining repetitions for gen_curr */

static void seq_ensure(size_t needed)
/* Allocate more memory for seq_arr if required. */
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* out of memory */
    seq_arr = tmp;
    seq_cap = new_cap;
}

static void init(void)
/* Initialise the sequence generation the first time it is needed. */
{
    if (seq_arr) return;           /* already initialised */

    seq_cap = 8;
    seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_arr) exit(1);

    seq_arr[0] = 2;                /* first term of the sequence */
    seq_len  = 1;
    gen_pos  = 0;
    gen_curr = 2;
    gen_run  = seq_arr[0] - 1;     /* remaining repeats after the first element */
}

/* ------------------------------------------------------------------ */
/*  public function -------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n = 0, but safe return */

    init();                       /* ensure the state is set up */

    /* Generate further terms until the desired index is reached. */
    while (seq_len < (size_t)n) {
        seq_ensure(seq_len + 1);   /* make space for the new element */
        seq_arr[seq_len] = gen_curr;
        seq_len++;

        gen_run--;
        if (gen_run == 0) {       /* finished a run – start the next run */
            gen_pos++;
            gen_curr = 3 - gen_curr;          /* toggle between 1 and 2 */
            gen_run  = seq_arr[gen_pos];
        }
    }

    return seq_arr[n - 1];
}
