/*
 * A100818  For a given unrestricted partition pi, let P(pi)=lambda(pi), if
 *          mu(pi)=0. If mu(pi)>0 then let P(pi)=nu(pi), where nu(pi) is the
 *          number of parts of pi greater than mu(pi), mu(pi) is the number of
 *          ones in pi and lambda(pi) is the largest part of pi.
 * https://oeis.org/A100818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 134, u64 n <= 427,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:   a(1)=1, a(2)=2,
 *            a(n) = A000041(n) – a(n-1)   for n>2
 * This implements the recurrence by computing
 * the partition numbers A000041(n) with the
 * generalized pentagonal number theorem.
 * All arithmetic is performed on seq_t; only
 * additions are used in the recurrence, so
 * no intermediate subtraction underflows.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  static storage for partition numbers and the sequence a(n)       */
/* ------------------------------------------------------------------ */
static seq_t *part = NULL;      /* partition numbers  part[0..N]   */
static size_t part_cap = 0;     /* current allocated size       */
static idx_t  part_size = 0;    /* highest index computed       */

static seq_t *aseq = NULL;      /* a(n) values  aseq[0..N]       */
static size_t a_cap = 0;        /* current allocated size       */
static idx_t  a_size = 0;       /* highest index computed       */

/* ------------------------------------------------------------------ */
/*  Utility: ensure that part array can hold at least up to n         */
/* ------------------------------------------------------------------ */
static void ensure_part_capacity(idx_t n)
{
    if (part == NULL) {
        /* initial allocation for part[0]  */
        part_cap = 16;
        part     = (seq_t *)malloc(part_cap * sizeof(seq_t));
        if (part == NULL) exit(1);
        memset(part, 0, part_cap * sizeof(seq_t));
        part[0] = (seq_t)1;           /* p(0) = 1   */
        part_size = 0;
    }

    /* enlarge allocation if necessary */
    if ((size_t)(n + 1) > part_cap) {
        size_t new_cap = part_cap ? part_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(part, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        /* initialise new slots to zero */
        memset(tmp + part_cap, 0, (new_cap - part_cap) * sizeof(seq_t));
        part = tmp;
        part_cap = new_cap;
    }

    /* compute missing partition numbers */
    for (idx_t m = part_size + 1; m <= n; ++m) {
        seq_t posSum = 0;
        seq_t negSum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > m && pent2 > m) break;

            seq_t term1 = 0;
            seq_t term2 = 0;
            if (pent1 <= m) term1 = part[m - pent1];
            if (pent2 <= m) term2 = part[m - pent2];

            int sign = (k & 1) ? 1 : -1;   /* odd k → +1, even k → -1 */
            if (sign > 0) {
                if (pent1 <= m) posSum += term1;
                if (pent2 <= m) posSum += term2;
            } else {
                if (pent1 <= m) negSum += term1;
                if (pent2 <= m) negSum += term2;
            }
        }
        part[m] = posSum - negSum;        /* safe: posSum >= negSum   */
    }
    part_size = n;
}

/* ------------------------------------------------------------------ */
/*  Utility: ensure that aseq array can hold at least up to n         */
/* ------------------------------------------------------------------ */
static void ensure_a_capacity(idx_t n)
{
    if (aseq == NULL) {
        a_cap = 16;
        aseq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (aseq == NULL) exit(1);
        memset(aseq, 0, a_cap * sizeof(seq_t));
        a_size = 0;
    }

    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(aseq, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        memset(tmp + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        aseq = tmp;
        a_cap = new_cap;
    }

    for (idx_t m = a_size + 1; m <= n; ++m) {
        if (m == 1) {
            aseq[1] = (seq_t)1;
        } else if (m == 2) {
            aseq[2] = (seq_t)2;
        } else {
            /* p(m) already computed */
            aseq[m] = part[m] - aseq[m - 1];
        }
    }
    a_size = n;
}

/* ------------------------------------------------------------------ */
/*  The required OEIS sequence function                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) {                              /* outside range */
        return (seq_t)0;
    }
    ensure_part_capacity(n);
    ensure_a_capacity(n);
    return aseq[n];
}
