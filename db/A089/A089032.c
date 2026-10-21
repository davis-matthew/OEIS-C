/*
 * A089032  Values i in primitive Pythagorean triangles (3i,4j,5k), with
 *          3i<4j<5k, sorted on k, then on j.
 * https://oeis.org/A089032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate all primitive Pythagorean triples (a,b,c) with
   a=m²−n², b=2mn, c=m²+n², m>n>0, one odd one even, gcd(m,n)=1.
   Keep those with a divisible by 3, b divisible by 4, c divisible by 5,
   and satisfying a<b<c (i.e., 3i<4j<5k).
   For each such triple store i=a/3 with key k=c/5 and j=b/4.
   Sort the collected triples by increasing k, then by increasing j,
   and output the i-values in that order.  This yields A089032. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Fast gcd for 64‑bit integers ------------------------------------------------*/
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Term used during sorting -----------------------------------------------------*/
typedef struct {
    seq_t  i;   /* the sequence value to return */
    uint32_t k; /* primary sort key   (c/5) */
    uint32_t j; /* secondary sort key (b/4) */
} Term;

/* Comparator for qsort ----------------------------------------------------------*/
static int cmp_terms(const void *p1, const void *p2)
{
    const Term *t1 = (const Term *)p1;
    const Term *t2 = (const Term *)p2;
    if (t1->k != t2->k) return (t1->k < t2->k) ? -1 : 1;
    if (t1->j != t2->j) return (t1->j < t2->j) ? -1 : 1;
    return 0;
}

/* Static storage for the generated sequence -----------------------------------*/
static seq_t *i_vals     = NULL;   /* sorted i values  */
static size_t values_len = 0;      /* number of valid terms */

/* Generate all terms up to a fixed maximum hypotenuse C_LIMIT. -------------------*/
static void generate_all(void)
{
    /* Choose C_LIMIT large enough to produce well over 10 000 terms.           */
    /* Rough estimate: for 20 000 000 the number of primitive triples is
       ≈ 20 000 000/(2π²) ≈ 1 000 000; filtering by 3|a, 4|b, 5|c leaves
       ≈ 1 000 000/60 ≈ 16 700 terms, comfortably above 10 000.                */
    const uint32_t C_LIMIT = 20000000U;

    /* Dynamically grow the array of terms.                                      */
    size_t term_cap = 40000;
    size_t term_len = 0;
    Term *terms = (Term *)malloc(term_cap * sizeof(Term));
    if (!terms) exit(1);

    /* Enumerate Euclid parameters m,n generating primitive triples.           */
    for (uint32_t m = 2; ; ++m) {
        uint64_t m2 = (uint64_t)m * m;
        if (m2 > C_LIMIT) break;                     /* further m gives c > C_LIMIT */
        for (uint32_t n = 1; n < m; ++n) {
            uint64_t n2 = (uint64_t)n * n;
            uint64_t c_val = m2 + n2;
            if (c_val > C_LIMIT) break;              /* larger n only increases c */

            /* Primitive condition: opposite parity, coprime */
            if (((m ^ n) & 1) == 0) continue;        /* same parity -> not primitive */
            if (gcd64(m, n) != 1) continue;          /* not coprime -> not primitive */

            /* Primitive triple components */
            uint32_t a_val = (uint32_t)(m2 - n2);                 /* a = m²−n²   */
            uint32_t b_val = (uint32_t)(2ULL * m * n);            /* b = 2mn     */
            uint32_t c_val32 = (uint32_t)c_val;                  /* c = m²+n²   */

            /* Required divisibility tests */
            if (a_val % 3 != 0) continue;              /* 3i | a */
            if (c_val32 % 5 != 0) continue;            /* 5k | c */

            /* Enforce a<b<c  (i.e., 3i<4j<5k) */
            if (a_val >= b_val) continue;              /* a must be smaller than b */

            /* Term meets all criteria – store it */
            if (term_len == term_cap) {
                term_cap *= 2;
                terms = (Term *)realloc(terms, term_cap * sizeof(Term));
                if (!terms) exit(1);
            }
            Term *t = &terms[term_len++];
            t->i = (seq_t)(a_val / 3);      /* i = a/3 */
            t->k = c_val32 / 5;             /* k = c/5 */
            t->j = b_val / 4;               /* j = b/4 */
        }
    }

    /* Sort by k then by j */
    qsort(terms, term_len, sizeof(Term), cmp_terms);

    /* Transfer sorted i-values to the static output array */
    i_vals = (seq_t *)malloc(term_len * sizeof(seq_t));
    if (!i_vals) exit(1);
    for (size_t i = 0; i < term_len; ++i) i_vals[i] = terms[i].i;
    values_len = term_len;

    free(terms);
}

/* Sequence function requested by the problem ---------------------------------*/
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        generate_all();
        initialized = true;
    }
    if (n == 0 || (idx_t)n > values_len) return 0;   /* out of bounds */
    return i_vals[n - 1];
}
