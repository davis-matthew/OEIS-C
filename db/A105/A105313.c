/*
 * A105313  Triangle read by rows, based on the morphism f: 1->432, 2->431,
 *          3->421, 4->321. First row is 1. If current row is a,b,c,..., then
 *          the next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A105313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Triangle read by rows → sequence obtained by extending the
   current row a,b,c,… with f(a),f(b),f(c),…, where
   f(1)=432, f(2)=431, f(3)=421, f(4)=321.
   Base row: a(0)=1.  The function returns a(n) for 0 ≤ n.  It works
   for all allowed seq_t and idx_t types. */

#include <stdint.h>

static const seq_t f_map[5][3] = {
    {0, 0, 0},          /* unused index 0 */
    {4, 3, 2},          /* f(1) */
    {4, 3, 1},          /* f(2) */
    {4, 2, 1},          /* f(3) */
    {3, 2, 1}           /* f(4) */
};

static seq_t get_val(idx_t r, idx_t pos)
{
    if (r == 0)
        return 1;                /* row 0 consists only of 1 */

    /* length of previous row: 4^(r-1) */
    idx_t lenPrev = 1;
    for (idx_t i = 1; i < r; ++i)
        lenPrev *= 4;

    if (pos < lenPrev)
        return get_val(r - 1, pos);

    idx_t offset = pos - lenPrev;     /* index within the appended block */
    idx_t p      = offset / 3;        /* which element of previous row */
    idx_t s      = offset % 3;        /* which digit of f() to take */

    seq_t prev = get_val(r - 1, p);
    int pv = (int)prev;               /* safe cast – prev is in {1,2,3,4} */
    return f_map[pv][s];
}

seq_t a(idx_t n)
{
    idx_t cum = 0;   /* cumulative length of rows before current */
    idx_t len = 1;   /* length of current row (4^r) */
    idx_t r   = 0;   /* current row number */

    /* Find the row that contains the nth term */
    while (n >= cum + len) {
        cum += len;
        ++r;
        if (len > UINT64_MAX / 4)
            break;                  /* safety, though n ≤ 10^4 here */
        len *= 4;
    }

    idx_t pos = n - cum;              /* position within the found row */
    return get_val(r, pos);
}
