/*
 * A221733  Number of nX4 arrays of occupancy after each element stays put or
 *          moves to some horizontal or antidiagonal neighbor, without move-in
 *          move-out left turns.
 * https://oeis.org/A221733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   parse_bigint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A221733: # of n×4 arrays of occupancy after each element
   stays put or moves to a horizontal or antidiagonal neighbour,
   no left turns.

   The values satisfy the linear recurrence of order 12
   (valid for n > 13):
       a(n) = 65*a(n-1) + 82*a(n-2) - 286*a(n-3)
              - 9015*a(n-4) - 13638*a(n-5) - 20531*a(n-6)
              - 4247*a(n-7) - 4296*a(n-8) + 12223*a(n-9)
              + 1713*a(n-10) - 233*a(n-11) + 12*a(n-12)

   Initial terms (n = 1 … 13) are given explicitly below.
   The function will memoise computed terms so that calls may
   arrive in any order.
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/parse_bigint.h"

/*------------------------------------------------------------------*/
/* Simple parser for decimal strings into seq_t. Works with any  */
/* unsigned integer type that supports *, + and from int.          */

/*------------------------------------------------------------------*/
/* Memoisation buffer                                                   */
/*------------------------------------------------------------------*/
static seq_t *buf = NULL;      /* dynamic array of terms A(1)..A(k) */
static size_t buf_len = 0;     /* number of terms currently stored */
static size_t buf_cap = 0;     /* allocated capacity                 */

/* Ensure the buffer can hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (need <= buf_cap)
        return;
    size_t new_cap = buf_cap ? buf_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* allocation failure – terminate */
    buf = tmp;
    buf_cap = new_cap;
}

/*------------------------------------------------------------------*/
/* Initialise first 13 terms once.                                     */
/*------------------------------------------------------------------*/
static void init_first13(void)
{
    if (buf_len >= 13)
        return; /* already initialised */

    ensure_capacity(13);

    /* Decimal representations of the first 13 terms. */
    static const char *const init_str[13] = {
        "21",
        "1347",
        "89920",
        "5962648",
        "394328064",
        "26082119800",
        "1725138005232",
        "114104918937179",
        "7547183523011758",
        "499189511916458843",
        "33017637367931117507",
        "2183868753937529833974",
        "144446517516157120963359"
    };

    for (size_t i = 0; i < 13; ++i)
        buf[i] = parse_bigint(init_str[i]);

    buf_len = 13;
}

/*------------------------------------------------------------------*/
/* Compute a(n) for n >= 14 from the recurrence.  'idx' is the     */
/* 0‑based array index (i.e. a(1) -> buf[0]).                         */
/*------------------------------------------------------------------*/
static seq_t compute_from_recurrence(size_t idx)
{
    /* Constants of the recurrence, cast to seq_t. */
    const seq_t C65     = (seq_t)65;
    const seq_t C82     = (seq_t)82;
    const seq_t C286    = (seq_t)286;
    const seq_t C9015   = (seq_t)9015;
    const seq_t C13638  = (seq_t)13638;
    const seq_t C20531  = (seq_t)20531;
    const seq_t C4247   = (seq_t)4247;
    const seq_t C4296   = (seq_t)4296;
    const seq_t C12223  = (seq_t)12223;
    const seq_t C1713   = (seq_t)1713;
    const seq_t C233    = (seq_t)233;
    const seq_t C12     = (seq_t)12;

    /* Pull the 12 needed previous terms. */
    seq_t t1  = buf[idx - 1];
    seq_t t2  = buf[idx - 2];
    seq_t t3  = buf[idx - 3];
    seq_t t4  = buf[idx - 4];
    seq_t t5  = buf[idx - 5];
    seq_t t6  = buf[idx - 6];
    seq_t t7  = buf[idx - 7];
    seq_t t8  = buf[idx - 8];
    seq_t t9  = buf[idx - 9];
    seq_t t10 = buf[idx -10];
    seq_t t11 = buf[idx -11];
    seq_t t12 = buf[idx -12];

    seq_t val =  NULL; /* initialise to silence compiler warnings */

    val = t1  * C65
        + t2  * C82
        - t3  * C286
        - t4  * C9015
        - t5  * C13638
        - t6  * C20531
        - t7  * C4247
        - t8  * C4296
        + t9  * C12223
        + t10 * C1713
        - t11 * C233
        + t12 * C12;

    return val;
}

/*------------------------------------------------------------------*/
/* Public API required by the framework.                              */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset is undefined */
        return (seq_t)0;

    size_t idx = (size_t)n;   /* convert to 0‑based index internally */

    init_first13();          /* make sure first 13 terms exist */

    /* If the requested term is already computed, return it. */
    if (idx <= buf_len)
        return buf[idx - 1];

    /* Compute missing terms up to 'idx' and cache them. */
    for (size_t i = buf_len; i < idx; ++i) {
        ensure_capacity(i + 1);
        buf[i] = compute_from_recurrence(i);
    }
    buf_len = idx;
    return buf[idx - 1];
}
