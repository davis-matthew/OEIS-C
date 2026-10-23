/*
 * A169006  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n≥1: a(n)= 39·(a(n-1)+a(n-2)+…+a(n-22)) –
 *            780·a(n-23) + b(n), where b(0)=1, b(1)…b(22)=2, b(23)=1, b(n)=0
 *            for n≥24. This follows from the rational generating function:
 *            (t^23+2t^22+…+2t+1)/(1–39t–39t^2–…–39t^22+780t^23)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined elsewhere (unsigned integer types). */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* We need only 23 previous values: use a circular buffer. */
    seq_t buffer[23] = {0};

    /* Initialise the zero‑index value. */
    buffer[0] = (seq_t)1;          /* a(0) = 1 */

    /* Compute a(1) … a(n) iteratively. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;      /* Σ_{k=1}^{min(22,i)} a(i-k) */

        /* Compute the sum of the last up to 22 terms. */
        for (idx_t k = 1; k <= 22; ++k) {
            if (i >= k) {
                sum += buffer[(i - k) % 23];
            }
        }

        /* Apply the recurrence. */
        seq_t val = sum * (seq_t)39;          /* 39*sum */

        if (i >= 23) {                      /* subtract 780*a(i-23)   */
            val -= buffer[(i - 23) % 23] * (seq_t)780;
        }

        /* Add the constant term b(i). */
        if (i <= 22) {
            val += (seq_t)2;                /* b(i)=2 for 1≤i≤22     */
        } else if (i == 23) {
            val += (seq_t)1;                /* b(23)=1                */
        }
        /* For i≥24: b(i)=0, nothing to add. */

        /* Store the new term.  The index is i mod 23. */
        buffer[i % 23] = val;
    }

    /* Result is the element corresponding to index n. */
    return buffer[n % 23];
}
