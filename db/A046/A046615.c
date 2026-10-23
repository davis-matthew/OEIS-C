/*
 * A046615  First numerator and then denominator of the elements to the right
 *          of the central elements of the 1/5-Pascal triangle (by row),
 *          excluding 1's.
 * https://oeis.org/A046615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) is obtained by enumerating the fractions that appear to the
 *            right of the central element in each row of the 1/5‑Pascal
 *            triangle. For each such fraction we output the numerator first,
 *            and if the denominator is not 1 we immediately output the
 *            denominator (otherwise we output only the numerator...
 * Range:     u8 n <= 21, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a computed Pascal‑triangle‑like recurrence.
    Two rows are stored at a time; the right‑hand side of each row is
    scanned to generate the sequence entries.  Results are cached so
    that subsequent calls, even with non‑monotonic arguments, are
    fast.  */

#include <stdlib.h>

typedef struct {
    seq_t        num;   /* numerator (may be 1)   */
    seq_t        den;   /* 1 or 5                 */
} Frac;

/* ------------------------------------------------------------------ */
/*  Dynamic arrays for the two most recent rows                         */
/* ------------------------------------------------------------------ */
static Frac *prev_row = NULL;     /* row r-1 */
static size_t prev_len = 0;

static Frac *curr_row = NULL;     /* row r   */
static size_t curr_len = 0;

/* ------------------------------------------------------------------ */
/*  Dynamic array for the sequence terms                               */
/* ------------------------------------------------------------------ */
static seq_t *terms      = NULL;
static idx_t  terms_len  = 0;     /* number of terms stored */
static idx_t  terms_cap  = 0;     /* allocation capacity */

/* Add a new value to the terms array, growing it if necessary. */
static void push_term(seq_t v)
{
    if (terms_len >= terms_cap) {
        terms_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    /* --- initialise on first call ---------------------------------- */
    static int initialized = 0;
    static idx_t next_row = 4;      /* we start generating from row 4 */

    if (!initialized) {
        /* row 3: 1 , 1/5 , 1 */
        prev_len = 3;
        prev_row = (Frac *)malloc(prev_len * sizeof(Frac));
        prev_row[0].num = (seq_t)1;  prev_row[0].den = (seq_t)1;
        prev_row[1].num = (seq_t)1;  prev_row[1].den = (seq_t)5;
        prev_row[2].num = (seq_t)1;  prev_row[2].den = (seq_t)1;

        terms_len = 0;
        terms_cap = 0;
        terms = NULL;

        initialized = 1;
    }

    /* --- generate rows until we have at least n terms -------------- */
    while (terms_len < n) {
        idx_t r = next_row;          /* current row number  */
        size_t len = (size_t)r;      /* number of entries in row */

        /* Ensure curr_row has enough space. */
        if (curr_len < len) {
            curr_row = (Frac *)realloc(curr_row, len * sizeof(Frac));
            curr_len = len;
        }

        /* Edges are always 1/1. */
        curr_row[0].num = (seq_t)1;  curr_row[0].den = (seq_t)1;
        curr_row[len - 1].num = (seq_t)1;  curr_row[len - 1].den = (seq_t)1;

        /* Interior entries via the recurrence. */
        for (size_t k = 1; k + 1 < len; ++k) {
            Frac *a = &prev_row[k - 1];
            Frac *b = &prev_row[k];

            seq_t a_scaled = (a->den == (seq_t)5) ? a->num
                                                    : a->num * ((seq_t)5);
            seq_t b_scaled = (b->den == (seq_t)5) ? b->num
                                                    : b->num * ((seq_t)5);
            seq_t sum_num  = a_scaled + b_scaled;
            seq_t sum_den  = (seq_t)5;

            /* Reduce if divisible by 5. */
            if (sum_num % ((seq_t)5) == (seq_t)0) {
                sum_num /= (seq_t)5;
                sum_den = (seq_t)1;
            }

            curr_row[k].num = sum_num;
            curr_row[k].den = sum_den;
        }

        /* Determine the first index to the right of the centre. */
        size_t start = (r % 2 == 0) ? (len / 2) : (len / 2 + 1);

        /* Scan the right side and append terms. */
        for (size_t j = start; j < len; ++j) {
            Frac *f = &curr_row[j];
            if (f->num == (seq_t)1 && f->den == (seq_t)1)
                continue;                       /* skip 1/1 */

            push_term(f->num);                   /* numerator */
            if (f->den != (seq_t)1)              /* non‑trivial denominator */
                push_term(f->den);
        }

        /* Prepare for next iteration. */
        Frac *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
        prev_len = curr_len;
        next_row++;
    }

    return terms[n - 1];
}
