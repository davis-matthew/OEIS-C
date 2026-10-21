/*
 * A208905  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208906; see the Formula section.
 * https://oeis.org/A208905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The polynomials are defined by u(1,x)=1, v(1,x)=1 and for n>1
 *            u(n,x)=u(n-1,x)+2x*v(n-1,x) v(n,x)=x*u(n-1,x)+v(n-1,x)+1 The
 *            sequence a(n) is the flattening of the coefficients of u(k,x)
 *            (row k contains the coefficients of u(k,x) in increasing order
 *            of powers of x). Initial values: a(1)=1, a(2)=1,...
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(idx_t n) returns the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* The largest n handled comfortably by 200 rows (n≈10000). */
    const idx_t MAXK = 200;

    /* Working arrays for the current and previous rows. */
    seq_t u[MAXK], v[MAXK];
    seq_t new_u[MAXK], new_v[MAXK];

    /* Initialise first row. */
    u[0] = 1;
    v[0] = 1;

    if (n < 1) return 0;          /* not defined for n<1 */
    if (n == 1) return u[0];      /* a(1)=1 */

    /* Current length (number of coefficients in u/v). */
    idx_t curr_len = 1;

    /* Number of coefficients already output. */
    idx_t cumulative = 1;

    /* Next row will have length curr_len+1. */
    idx_t next_len = curr_len + 1;

    while (cumulative < n) {
        /* Compute the new row from the previous one. */
        new_u[0] = u[0];
        new_v[0] = v[0] + 1;

        for (idx_t i = 1; i < next_len; ++i) {
            seq_t u_prev = (i < curr_len ? u[i] : 0);
            new_u[i] = u_prev + (seq_t)2 * v[i - 1];

            seq_t term2 = (i - 1 < curr_len ? u[i - 1] : 0);
            seq_t term3 = (i < curr_len ? v[i] : 0);
            new_v[i] = term2 + term3;
        }

        /* Emit the new coefficients, checking for the requested index. */
        for (idx_t i = 0; i < next_len; ++i) {
            ++cumulative;
            if (cumulative == n)
                return new_u[i];
        }

        /* Prepare for the next iteration. */
        for (idx_t i = 0; i < next_len; ++i) {
            u[i] = new_u[i];
            v[i] = new_v[i];
        }

        curr_len = next_len;
        next_len = curr_len + 1;
    }

    /* Should never reach here. */
    return 0;
}
