/*
 * A182113  Lexicographically earliest permutation of the positive integers
 *          such that a(n+a(n)+1) has the same parity as a(n).
 * https://oeis.org/A182113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construct a(n) as the lexicographically earliest permutation of the positive
   integers such that a(n + a(n) + 1) has the same parity as a(n).
   Greedy search: for each n, choose the smallest unused x that
   satisfies any existing parity requirement at index n and that does
   not violate parity constraints for the future index m = n + x + 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper to grow a 1‑dimensional array of unsigned chars */
static void grow_uchar_array(unsigned char **arr, size_t *cap, size_t needed)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)calloc(new_cap, 1);
    if (*arr) {
        memcpy(tmp, *arr, *cap);
        free(*arr);
    }
    *arr = tmp;
    *cap = new_cap;
}

/* helper to grow a 1‑dimensional array of seq_t */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (*arr) {
        memcpy(tmp, *arr, *cap * sizeof(seq_t));
        free(*arr);
    }
    *arr = tmp;
    *cap = new_cap;
}

static seq_t *A  = NULL;          /* sequence values, 1‑indexed */
static size_t A_cap = 0;
static idx_t  max_calculated = 0; /* highest index computed */

static unsigned char *req = NULL; /* parity requirement: 0 none, 1 even, 2 odd */
static size_t req_cap = 0;

static unsigned char *used_vals = NULL; /* used numbers */
static size_t used_cap = 0;

/* compute the sequence up to target_n */
static void compute_to(idx_t target_n)
{
    if (target_n <= max_calculated) return;

    /* ensure capacity for A */
    grow_seq_array(&A, &A_cap, (size_t)target_n + 5);

    /* ensure capacity for req (to at least target_n) */
    grow_uchar_array(&req, &req_cap, (size_t)target_n + 5);

    while (max_calculated < target_n) {
        idx_t n = max_calculated + 1;        /* 1‑based current index */
        unsigned char req_par = (n < req_cap) ? req[n] : 0; /* 0 none, 1 even, 2 odd */

        seq_t cand = 1;                     /* candidate value */
        for (;;) {
            /* enlarge used array if needed */
            size_t cand_sz = (size_t)cand + 1;
            grow_uchar_array(&used_vals, &used_cap, cand_sz);

            if (used_vals[(size_t)cand]) {       /* already used */
                cand++;
                continue;
            }

            /* parity requirement at n? */
            if (req_par != 0) {
                bool odd_needed = (req_par == 2);
                if (((cand & 1) != 0) != odd_needed) {
                    cand++;
                    continue;
                }
            }

            idx_t m = n + (idx_t)cand + 1;        /* future index to check */

            bool conflict = false;
            if (m <= max_calculated) {
                /* m already assigned; check parity */
                if (((A[(size_t)m] & 1) != 0) != ((cand & 1) != 0))
                    conflict = true;
            } else {
                /* ensure req array can hold m */
                grow_uchar_array(&req, &req_cap, (size_t)m + 1);
                unsigned char r = req[(size_t)m];
                if (r != 0) {
                    bool odd_needed = (r == 2);
                    if (((cand & 1) != 0) != odd_needed)
                        conflict = true;
                }
            }

            if (!conflict) break;  /* candidate is acceptable */
            cand++;
        }

        /* assign the chosen value */
        A[(size_t)n] = cand;
        used_vals[(size_t)cand] = 1;

        /* set requirement at future index */
        idx_t m = n + (idx_t)cand + 1;
        grow_uchar_array(&req, &req_cap, (size_t)m + 1);
        if (!req[(size_t)m]) {                     /* set only if not set yet */
            req[(size_t)m] = (cand & 1) ? 2 : 1;    /* 2 = odd, 1 = even */
        }
        max_calculated++;
    }
}

/* public interface ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but not expected */
    compute_to(n);
    return A[(size_t)n];
}
